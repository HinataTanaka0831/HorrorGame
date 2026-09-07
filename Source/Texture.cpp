#include "Texture.h"

Texture::Texture(std::string fileName, VECTOR centerPosition, int transFlag)
    : m_position(centerPosition)
    , m_transFlag(transFlag)
{
    // 画像の読み込み
    m_handle = LoadGraph(fileName.c_str());

    // 画像のサイズを取得する
    GetGraphSize(m_handle, &m_sizeX, &m_sizeY);

    // 半径を設定（とりあえず共通で幅の半分を半径としておく）
    m_radius = (float)m_sizeX / 2.0f;
}

Texture::~Texture()
{
    // 読み込んだ画像の破棄
    DeleteGraph(m_handle);
}

void Texture::Draw()
{
    // 画像の表示
    DrawGraph((int)m_position.x - (m_sizeX / 2), (int)m_position.y - (m_sizeY / 2), m_handle, m_transFlag);
}

void Texture::Update()
{
    
}
