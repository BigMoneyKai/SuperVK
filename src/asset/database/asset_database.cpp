#include "asset/database/asset_database.h"

void AssetDatabase::clear() {
    m_meshAssetDB.clear();
    m_materialAssetDB.clear();
    m_textureAssetDB.clear();
    m_shaderAssetDB.clear();
    m_sceneAssetDB.clear();
    m_animationAssetDB.clear();
    m_skeletonAssetDB.clear();
}
