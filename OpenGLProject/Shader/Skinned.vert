#version 330

uniform mat4 uWorldTransform;
uniform mat4 uViewProj;
uniform mat4 uMatrixPalette[96];

// 属性
// 0：位置, 1 法線,
// 2：ボーンの順番, 3：重み,
// 4：テクスチャ座標.
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in uvec4 inSkinBones;
layout(location = 3) in vec4 inSkinWeights;
layout(location = 4) in vec2 inTexCoord;

out vec2 fragTexCoord;
// 法線
out vec3 fragNormal;
// ワールド座標
out vec3 fragWorldPos;

void main()
{
	// 位置を同次座標に変換
	vec4 pos = vec4(inPosition, 1.0);
	
	// 位置のスキニング
	vec4 skinnedPos = (pos * uMatrixPalette[inSkinBones.x]) * inSkinWeights.x;
	skinnedPos += (pos * uMatrixPalette[inSkinBones.y]) * inSkinWeights.y;
	skinnedPos += (pos * uMatrixPalette[inSkinBones.z]) * inSkinWeights.z;
	skinnedPos += (pos * uMatrixPalette[inSkinBones.w]) * inSkinWeights.w;

	// 位置をワールド空間に変換
	skinnedPos = skinnedPos * uWorldTransform;
	// ワールドポジションを保存
	fragWorldPos = skinnedPos.xyz;
	// クリップ空間に変換
	gl_Position = skinnedPos * uViewProj;

	// 頂点法線のスキニング
	vec4 skinnedNormal = vec4(inNormal, 0.0f);
	skinnedNormal = (skinnedNormal * uMatrixPalette[inSkinBones.x]) * inSkinWeights.x
		+ (skinnedNormal * uMatrixPalette[inSkinBones.y]) * inSkinWeights.y
		+ (skinnedNormal * uMatrixPalette[inSkinBones.z]) * inSkinWeights.z
		+ (skinnedNormal * uMatrixPalette[inSkinBones.w]) * inSkinWeights.w;
	// 法線をワールド空間に変換(w=0)
	fragNormal = (skinnedNormal * uWorldTransform).xyz;

	// テクスチャ座標をフラグメントシェーダーに渡す
	fragTexCoord = inTexCoord;
}

