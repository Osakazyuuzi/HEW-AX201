// CPUから受け取った頂点データ
struct VS_IN
{
	float3 pos : POSITION0;
	float2 uv : TEXCOORD0;
};

// ピクセルシェーダーに渡すデータ
// ※SV_POSITIONはラスタライズに渡される
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

cbuffer Transform : register(b0)
{
	float posX, posY;
	float scaleX, scaleY;
	float angle;
	float dummy[3];
}

VS_OUT main(VS_IN vin)
{
	VS_OUT vout;
	vout.uv = vin.uv;	// ピクセルシェーダへ受け渡し

	vout.pos = float4(vin.pos, 1.0f);

	//--- ポリゴンの変形
	// 拡縮
	vout.pos.x *= scaleX;
	vout.pos.y *= scaleY;
	// 回転
	float4 work = vout.pos;
	vout.pos.x = work.x * cos(angle) - work.y * sin(angle);
	vout.pos.y = work.x * sin(angle) + work.y * cos(angle);
	// 移動
	vout.pos.x += posX;
	vout.pos.y += posY;


	// ①画面の上下をひっくり返す
	// 　-1をかけて上下の+-を反転
	vout.pos.y *= -1.0f;
	// ②画面のスケールを小さくする
	// 例：-1～1の間は"2"離れている。
	// 　　この間"2"にそろえるために、画面幅(960, 540)を、半分の大きさ(480, 270)で割る
	vout.pos.x /= 480.0f;
	vout.pos.y /= 270.0f;
	// ③(0, 0)の座標を左上(-1, 1)に移動
	vout.pos.x -= 1.0f;
	vout.pos.y += 1.0f;

	return vout;
}