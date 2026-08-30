#pragma once

// インベントリで保持するアイテム情報（識別ID、アイコン画像ハンドル、名称）
struct ItemData
{
	int id;              // アイテム識別番号
	int imageHandle;     // UI描画用テクスチャハンドル
	char name[32];       // アイテム表示名
};