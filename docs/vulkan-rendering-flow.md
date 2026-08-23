# Vulkan 与 SuperVK 渲染架构流程图

> 本文档配套 render graph 学习使用：上半是 Vulkan 从创建到销毁的完整流程，下半是你仓库（SuperVK）现在的分层与每帧的实际调用链，最后是一帧的逐步走查。

## 一、Vulkan 完整生命周期

```mermaid
flowchart TD
    subgraph INIT["① 初始化：只做一次，按依赖顺序创建全部对象"]
        I1["Instance：与驱动握手，指定应用名，加载验证层"]
        I2["Surface：连接窗口系统（GLFW），拿到可绘制表面"]
        I3["Physical Device：枚举 GPU，查特性、队列族"]
        I4["Logical Device：创建逻辑设备，拿 graphics + present 队列"]
        I5["Swapchain：创建 N 张交换链图像（颜色缓冲）+ ImageView"]
        I6["RenderPass：声明附件（color/depth）和 Load/Store 语义"]
        I7["Pipeline：编译 shader + 顶点布局 + 光栅化/混合/深度状态"]
        I8["Framebuffer：RenderPass × 具体图像，每张交换链图一个"]
        I9["资源：Buffer（顶点/UBO）、Image/Texture、DescriptorSet"]
        I10["同步：Fence（CPU等GPU）、Semaphore（GPU内部接力）"]
        I1 --> I2 --> I3 --> I4 --> I5
        I5 --> I6 --> I7
        I5 --> I8
        I4 --> I9
        I9 --> I10
        I6 --> I8
    end

    subgraph FRAME["② 帧循环：每帧重复同一套五步"]
        F1["等本帧槽位的 Fence：上一帧同一槽位画完了吗"]
        F2["Acquire：拿一张交换链图，信号 imageAvailable"]
        F3["Record：begin → barrier → BeginRenderPass → 绑管线/描述符 → draw → End → end"]
        F4["Submit：等 imageAvailable 才开始画，画完信号 renderFinished + Fence"]
        F5["Present：等 renderFinished，把图像交给屏幕"]
        F1 --> F2 --> F3 --> F4 --> F5
        F2 -.->|"N 张图轮着用，MAX_FRAMES_IN_FLIGHT 套资源"| F3
    end

    DESTROY["③ 销毁：严格逆序<br/>Framebuffer → RenderPass → Pipeline → 资源 → Command → Swapchain → Device → Surface → Instance"]

    INIT --> FRAME --> DESTROY
```

**要点**：创建顺序就是依赖顺序，销毁顺序就是它的逆序；帧循环只是中间那段五步舞曲的无限重复。所有"释放复杂"的焦虑，本质都是想把第三段做好。

## 二、SuperVK 现在的架构与一帧的运行

```mermaid
flowchart TD
    subgraph APP["App（app/app.cpp）"]
        A1["JobSystem：异步资产线程池"]
        A2["WinMan：GLFW 窗口"]
        A3["AssetMan：加载 GLB → 顶点/索引搬到 Scene"]
        A5["Scene：Camera / Object / Light / Material / Mesh"]
    end

    subgraph REN["Renderer（render/renderer.h）"]
        R0["Instance / Device / Surface / Swapchain"]
        R1["CommandPool + CommandBuffer（每帧槽位一个）"]
        R2["PipelineMan：用 graph 的 renderPassOf(0) 创建管线"]
        R3["DescriptorMan：Camera/Object/Light/Material UBO + 纹理"]
        R4["BufferMan / TextureMan"]
        R5["FrameResource：Fence ×1 + Semaphore ×2 + 帧计数"]
        R6["DepthResource：深度图 / 内存 / 视图"]
        R7["RenderGraph"]
    end

    subgraph GRAPH["RenderGraph 内部（render/graph）"]
        G1["addPass 声明：color/depth/input 附件 + drawList 回调"]
        G2["process()：登记资源用途 → 依赖边 → 拓扑排序 → 推导 barrier → 缓存 RenderPass"]
        G3["execute(cmd)：按序 barrier → BeginRenderPass → drawList() → End → 呈现目标切 PRESENT_SRC"]
        G1 --> G2 --> G3
    end

    subgraph DRAW["Main pass 的 drawList 回调"]
        D1["更新 4 个 UBO（读 Scene + 写帧缓冲）"]
        D2["绑 Pipeline + DescriptorSet"]
        D3["设 Viewport / Scissor"]
        D4["绑顶点/索引缓冲 → vkCmdDrawIndexed"]
    end

    subgraph LOOP["drawFrame 每帧"]
        L1["等 Fence → Acquire → 重置 Fence"]
        L2["setPresentTarget：喂这一帧的交换链图/视图"]
        L3["begin cmd → process() → execute() → end cmd"]
        L4["Submit（等 imageAvailable，信号 renderFinished）→ Present"]
        L5["m_currFrame = (m_currFrame + 1) % MAX_FRAMES_IN_FLIGHT"]
    end

    APP --> REN
    A5 --> DRAW
    R2 --> DRAW
    R3 --> DRAW
    R4 --> DRAW
    R7 --> GRAPH
    G2 -->|"renderPassOf(0)"| R2
    GRAPH --> LOOP
    L3 --> G2
    L3 --> G3
    G3 --> DRAW
```

**三个容易看漏的细节**：

1. **管线挂在图的 render pass 上**（`G2 → R2` 那条边）：init 里必须先 `addPass` + `process()`，管线才能拿到 `renderPassOf(0)` 创建。
2. **process 和 execute 的分工就是 render graph 模型**：`process()` = 声明变成排序和 barrier，`execute()` = 真正录制。两者都在 `drawFrame` 里每帧被调用，中间夹着 `setPresentTarget`。
3. **三件同步工具的职责**：Fence 管"同一槽位的上一帧"；`imageAvailable` semaphore 管"acquire 的图可以开始画了"；`renderFinished` semaphore 管"画完才能 present"。两个帧槽位轮换，所以 CPU 录第 N+1 帧时 GPU 正在画第 N 帧。

## 三、一帧的逐步走查（CPU / GPU 接力）

| 步 | 侧  | 动作             | 说明                                                                                            |
| -- | --- | ---------------- | ----------------------------------------------------------------------------------------------- |
| 1  | CPU | 等 Fence         | 等上一帧同一槽位画完，确认 GPU 已用完这套 FrameResource，CPU 才能安全写入                       |
| 2  | CPU | Acquire 交换链图 | `vkAcquireNextImageKHR` 拿一张交换链图；GPU 准备好后信号 `imageAvailable`                   |
| 3  | CPU | 录制命令         | begin → 图已算好的 barrier → BeginRenderPass → 绑管线/描述符 → draw → EndRenderPass → end |
| 4  | CPU | Submit           | `vkQueueSubmit`：等 `imageAvailable` 才开始执行；完成时信号 `renderFinished` + Fence      |
| 5  | GPU | 执行绘制         | 顶点 → 着色器 → 光栅化 → 片元，写入交换链图                                                  |
| 6  | GPU | 完成             | 信号`renderFinished`（Present 等它）和 Fence（下一轮 CPU 等它）                               |
| 7  | CPU | Present          | `vkQueuePresentKHR`：等 `renderFinished`，把图交给屏幕                                      |
| 8  | CPU | 下一帧           | 帧槽位 +1 取模；CPU 录下一帧时，GPU 可能还在画这一帧——这就是双缓冲                            |

## 四、还需要知道的细节

- shader 由 `glslangValidator` 预编译成 SPIR-V，构建时复制到可执行目录，因此**必须从仓库根运行**引擎。
- Debug 构建开着 ASan；`VK_CHECK_RESULT` 失败会打印全部缓冲日志后退出（Release 里是 no-op，所以验证要在 Debug）。
- 窗口缩放目前没有 swapchain 重建处理，`VK_ERROR_OUT_OF_DATE_KHR` 会直接退出——这是下一步要补的。
- framebuffer 目前按呈现图每帧重建（MVP 偷懒）；下一步应改为按 `(pass, imageIndex)` 缓存，把创建次数从"每帧 N 次"降到"总共 N 次"。
- render graph 的 barrier 目前用"全命令 + 内存写"做 src（过度同步但正确），跑通后再按附件槽位收紧。
