// CPU����󂯎�������_�f�[�^
struct VS_IN
{
	float3 pos : POSITION0;
	float2 uv : TEXCOORD0;
};

// �s�N�Z���V�F�[�_�[�ɓn���f�[�^
// ��SV_POSITION�̓��X�^���C�Y�ɓn�����
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
	vout.uv = vin.uv;	// �s�N�Z���V�F�[�_�֎󂯓n��

	vout.pos = float4(vin.pos, 1.0f);

	//--- �|���S���̕ό`
	// �g�k
	vout.pos.x *= scaleX;
	vout.pos.y *= scaleY;
	// ��]
	float4 work = vout.pos;
	vout.pos.x = work.x * cos(angle) - work.y * sin(angle);
	vout.pos.y = work.x * sin(angle) + work.y * cos(angle);
	// �ړ�
	vout.pos.x += posX;
	vout.pos.y += posY;


	// �@��ʂ̏㉺���Ђ�����Ԃ�
	// �@-1�������ď㉺��+-�𔽓]
	vout.pos.y *= -1.0f;
	// �A��ʂ̃X�P�[��������������
	// ��F-1�`1�̊Ԃ�"2"����Ă���B
	// �@�@���̊�"2"�ɂ��낦�邽�߂ɁA��ʕ�(960, 540)���A�����̑傫��(480, 270)�Ŋ���
	vout.pos.x /= 480.0f;
	vout.pos.y /= 270.0f;
	// �B(0, 0)�̍��W������(-1, 1)�Ɉړ�
	vout.pos.x -= 1.0f;
	vout.pos.y += 1.0f;

	return vout;
}