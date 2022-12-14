#version 460

#define SAMPLES_COUNT 128

layout (location = 0) in vec2 vTexCoord;

layout(location = 0) uniform int u_kernel_size;
layout(location = 1) uniform float u_ssao_radius;
layout(location = 2) uniform float u_ssao_bias;
layout(location = 3) uniform vec2 u_noise_size;
layout(location = 4) uniform vec3 u_samples[SAMPLES_COUNT];

layout(binding = 0) uniform sampler2D gPosition;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gNoise;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjectionMatrix;
    mat4 u_ViewMatrix;
    mat4 u_ProjectionMatrix;
    vec3 u_Position;
} camera;

layout (location = 0) out float FragColor;

void main() {
	vec3 frag_pos = texture(gPosition, vTexCoord).rgb;
	vec3 normal = normalize(texture(gNormal, vTexCoord).rgb);
	vec3 random_vec = normalize(texture(gNoise, vTexCoord * u_noise_size).xyz);

	vec3 tangent = normalize(random_vec - normal * dot(random_vec, normal));
	vec3 bi_tangent = cross(normal, tangent);
	mat3 tbn = mat3(tangent, bi_tangent, normal);

	float ssao_occlusion = 0.0;

	for (int i = 0; i < u_kernel_size; i++) {
		vec3 sample_pos = tbn * u_samples[i];
		sample_pos = frag_pos + sample_pos * u_ssao_radius;

		vec4 offset = vec4(sample_pos, 1.0);
		offset = camera.u_ViewProjectionMatrix * offset;
		offset.xyz /= offset.w; // perspective divide
    offset.xy = offset.xy * 0.5 + 0.5; // transform to range 0.0 - 1.0

		float sample_depth = texture(gPosition, offset.xy).z; 

		// range check & accumulate
    float range_check = smoothstep(0.0, 1.0, u_ssao_radius / abs(frag_pos.z - sample_depth));
    ssao_occlusion += (sample_depth >= sample_pos.z + u_ssao_bias ? 1.0 : 0.0) * range_check;    
	}
	ssao_occlusion = 1.0 - ssao_occlusion / float(u_kernel_size);

	FragColor = ssao_occlusion;
}