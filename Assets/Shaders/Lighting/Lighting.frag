#version 460

const int NUM_CASCADES = 3;

// Tex coords from vertex shader.
layout(location = 0) in vec2 TexCoords;

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float radius;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

// Camera UB
layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjectionMatrix;
    mat4 u_ViewMatrix;
    mat4 u_ProjectionMatrix;
    vec3 u_Position;
} camera;

// Lights UB
layout(std140, binding = 1) uniform Lights
{
    PointLight pointLight;
    DirectionalLight directionalLight;
} lights;

//GBuffer samplers
layout(binding = 0) uniform sampler2D gPosition;
layout(binding = 1) uniform sampler2D gAlbedo;
layout(binding = 2) uniform sampler2D gNormal;
layout(binding = 3) uniform sampler2D gRoughMetalAO;
layout(binding = 4) uniform sampler2D gViewPosition;

layout(binding = 5) uniform samplerCube gIrradiance;
layout(binding = 6) uniform samplerCube gPrefilter;
layout(binding = 7) uniform sampler2D gBRDFLut;
layout(binding = 8) uniform sampler2DArray       gShadowMap;
layout(binding = 9) uniform sampler2DArrayShadow gShadowMapPCF;
layout(binding = 10) uniform sampler3D            gRandomAngles;

float light_near_plane;
float light_far_plane;
float light_radius;
float correction_factor = 1.0;

uniform int   u_blocker_search_samples;
uniform float u_light_radius_uv;
uniform int   u_pcf_samples;
uniform vec2  u_light_frustum_planes[NUM_CASCADES];
uniform float u_cascade_splits[NUM_CASCADES];

uniform mat4 u_light_view_projections[NUM_CASCADES];
uniform mat4 u_light_views[NUM_CASCADES];

vec4 out_pos_light_view_space[NUM_CASCADES];
vec4 out_pos_light_clip_space[NUM_CASCADES];

layout(location = 0) out vec4 FragColor;

const float PI = 3.14159265359;

const vec2 Poisson25[25] = vec2[](
vec2(-0.978698,  -0.0884121),
vec2(-0.841121,   0.521165),
vec2(-0.71746,   -0.50322),
vec2(-0.702933,   0.903134),
vec2(-0.663198,   0.15482),
vec2(-0.495102,  -0.232887),
vec2(-0.364238,  -0.961791),
vec2(-0.345866,  -0.564379),
vec2(-0.325663,   0.64037),
vec2(-0.182714,   0.321329),
vec2(-0.142613,  -0.0227363),
vec2(-0.0564287, -0.36729),
vec2(-0.0185858,  0.918882),
vec2( 0.0381787, -0.728996),
vec2( 0.16599,    0.093112),
vec2( 0.253639,   0.719535),
vec2( 0.369549,  -0.655019),
vec2( 0.423627,   0.429975),
vec2( 0.530747,  -0.364971),
vec2( 0.566027,  -0.940489),
vec2( 0.639332,   0.0284127),
vec2( 0.652089,   0.669668),
vec2( 0.773797,   0.345012),
vec2( 0.968871,   0.840449),
vec2( 0.991882,  -0.657338));

const vec2 Poisson32[32] = vec2[](
vec2(-0.975402,  -0.0711386),
vec2(-0.920347,  -0.41142),
vec2(-0.883908,   0.217872),
vec2(-0.884518,   0.568041),
vec2(-0.811945,   0.90521),
vec2(-0.792474,  -0.779962),
vec2(-0.614856,   0.386578),
vec2(-0.580859,  -0.208777),
vec2(-0.53795,    0.716666),
vec2(-0.515427,   0.0899991),
vec2(-0.454634,  -0.707938),
vec2(-0.420942,   0.991272),
vec2(-0.261147,   0.588488),
vec2(-0.211219,   0.114841),
vec2(-0.146336,  -0.259194),
vec2(-0.139439,  -0.888668),
vec2( 0.0116886,  0.326395),
vec2( 0.0380566,  0.625477),
vec2( 0.0625935, -0.50853),
vec2( 0.125584,   0.0469069),
vec2( 0.169469,  -0.997253),
vec2( 0.320597,   0.291055),
vec2( 0.359172,  -0.633717),
vec2( 0.435713,  -0.250832),
vec2( 0.507797,  -0.916562),
vec2( 0.545763,   0.730216),
vec2( 0.56859,    0.11655),
vec2( 0.743156,  -0.505173),
vec2( 0.736442,  -0.189734),
vec2( 0.843562,   0.357036),
vec2( 0.865413,   0.763726),
vec2( 0.872005,  -0.927));

const vec2 Poisson64[64] = vec2[](
vec2(-0.934812,    0.366741),
vec2(-0.918943,   -0.0941496),
vec2(-0.873226,    0.62389),
vec2(-0.8352,      0.937803),
vec2(-0.822138,   -0.281655),
vec2(-0.812983,    0.10416),
vec2(-0.786126,   -0.767632),
vec2(-0.739494,   -0.535813),
vec2(-0.681692,    0.284707),
vec2(-0.61742,    -0.234535),
vec2(-0.601184,    0.562426),
vec2(-0.607105,    0.847591),
vec2(-0.581835,   -0.00485244),
vec2(-0.554247,   -0.771111),
vec2(-0.483383,   -0.976928),
vec2(-0.476669,   -0.395672),
vec2(-0.439802,    0.362407),
vec2(-0.409772,   -0.175695),
vec2(-0.367534,    0.102451),
vec2(-0.35313,     0.58153),
vec2(-0.341594,   -0.737541),
vec2(-0.275979,    0.981567),
vec2(-0.230811,    0.305094),
vec2(-0.221656,    0.751152),
vec2(-0.214393,   -0.0592364),
vec2(-0.204932,   -0.483566),
vec2(-0.183569,   -0.266274),
vec2(-0.123936,   -0.754448),
vec2(-0.0859096,   0.118625),
vec2(-0.0610675,   0.460555),
vec2(-0.0234687,  -0.962523),
vec2(-0.00485244, -0.373394),
vec2( 0.0213324,   0.760247),
vec2( 0.0359813,  -0.0834071),
vec2( 0.0877407,  -0.730766),
vec2( 0.14597,     0.281045),
vec2( 0.18186,    -0.529649),
vec2( 0.188208,   -0.289529),
vec2( 0.212928,    0.063509),
vec2( 0.23661,     0.566027),
vec2( 0.266579,    0.867061),
vec2( 0.320597,   -0.883358),
vec2( 0.353557,    0.322733),
vec2( 0.404157,   -0.651479),
vec2( 0.410443,   -0.413068),
vec2( 0.413556,    0.123325),
vec2( 0.46556,    -0.176183),
vec2( 0.49266,     0.55388),
vec2( 0.506333,    0.876888),
vec2( 0.535875,   -0.885556),
vec2( 0.615894,    0.0703452),
vec2( 0.637135,   -0.637623),
vec2( 0.677236,   -0.174291),
vec2( 0.67626,     0.7116),
vec2( 0.686331,   -0.389935),
vec2( 0.691031,    0.330729),
vec2( 0.715629,    0.999939),
vec2( 0.8493,     -0.0485549),
vec2( 0.863582,   -0.85229),
vec2( 0.890622,    0.850581),
vec2( 0.898068,    0.633778),
vec2( 0.92053,    -0.355693),
vec2( 0.933348,   -0.62981),
vec2( 0.95294,     0.156896));

const vec2 Poisson100[100] = vec2[](
vec2(-0.9891574,   -0.1059512),
vec2(-0.9822294,    0.05140843),
vec2(-0.961332,     0.2562195),
vec2(-0.9149657,   -0.2404464),
vec2(-0.8896608,   -0.4183828),
vec2(-0.8398135,    0.3748641),
vec2(-0.8149028,    0.1989844),
vec2(-0.8046502,    0.5212684),
vec2(-0.7970151,   -0.5834194),
vec2(-0.7484995,   -0.3153634),
vec2(-0.738582,    -0.09323367),
vec2(-0.695694,     0.08865929),
vec2(-0.6868832,    0.6336682),
vec2(-0.6751406,    0.2777427),
vec2(-0.666558,    -0.6801786),
vec2(-0.631489,    -0.4702293),
vec2(-0.5870083,    0.518836),
vec2(-0.5744062,   -0.06333278),
vec2(-0.5667221,    0.1699501),
vec2(-0.5537653,    0.7677022),
vec2(-0.5337034,    0.3299558),
vec2(-0.5201509,   -0.2033358),
vec2(-0.4873925,   -0.8545401),
vec2(-0.4712743,   -0.3607009),
vec2(-0.4524891,   -0.5142469),
vec2(-0.4421883,   -0.6830674),
vec2(-0.4293752,    0.6299667),
vec2(-0.4240644,    0.8706763),
vec2(-0.4139857,    0.1598689),
vec2(-0.3838707,    0.4078749),
vec2(-0.3688077,   -0.0358762),
vec2(-0.3432877,   -0.2311365),
vec2(-0.3256257,   -0.9325441),
vec2(-0.2751555,    0.302412),
vec2(-0.2679778,   -0.654425),
vec2(-0.2554769,   -0.4441924),
vec2(-0.243476,    -0.8034022),
vec2(-0.2367678,   -0.108045),
vec2(-0.2196257,    0.8243803),
vec2(-0.2119443,    0.06230118),
vec2(-0.1708038,   -0.9437978),
vec2(-0.1694005,    0.5692244),
vec2(-0.136494,     0.3937041),
vec2(-0.1318274,   -0.2166154),
vec2(-0.09781472,  -0.5743775),
vec2(-0.09480921,   0.2369129),
vec2(-0.07638182,  -0.0571501),
vec2(-0.06661344,  -0.7966294),
vec2(-0.06305461,  -0.3521975),
vec2(-0.04525706,   0.6982157),
vec2(-0.04149697,   0.9666064),
vec2(-0.003192461, -0.9693027),
vec2( 0.0104818,    0.5000805),
vec2( 0.03228819,  -0.1681713),
vec2( 0.03715288,  -0.673852),
vec2( 0.08470399,  -0.3922319),
vec2( 0.09848712,  -0.8374477),
vec2( 0.09940207,   0.1117471),
vec2( 0.1395643,    0.313647),
vec2( 0.1565993,    0.8555924),
vec2( 0.1772605,   -0.5248074),
vec2( 0.1899546,    0.5249656),
vec2( 0.1952665,   -0.9595091),
vec2( 0.213078,    -0.07045701),
vec2( 0.2277649,   -0.3361143),
vec2( 0.247221,     0.7353553),
vec2( 0.2493455,   -0.6874771),
vec2( 0.269915,     0.07673722),
vec2( 0.3039587,    0.9087375),
vec2( 0.3189922,    0.3008468),
vec2( 0.3215453,   -0.1954931),
vec2( 0.3593478,    0.4527411),
vec2( 0.3745022,   -0.597945),
vec2( 0.3879738,   -0.7821383),
vec2( 0.4522015,    0.6819367),
vec2( 0.4591872,   -0.4484442),
vec2( 0.4626173,   -0.03955235),
vec2( 0.4751598,    0.2083394),
vec2( 0.4894366,    0.8694122),
vec2( 0.4896614,   -0.2676601),
vec2( 0.5070116,   -0.6733028),
vec2( 0.5525513,    0.436241),
vec2( 0.5542312,   -0.8262905),
vec2( 0.6012187,    0.7003717),
vec2( 0.6075609,   -0.1610506),
vec2( 0.6291932,    0.2213627),
vec2( 0.6300695,   -0.5324634),
vec2( 0.6613995,   -0.7056449),
vec2( 0.6699739,   -0.3828001),
vec2( 0.6705787,    0.01011722),
vec2( 0.6814164,    0.5618623),
vec2( 0.7808329,    0.261445),
vec2( 0.7830279,   -0.1817809),
vec2( 0.8006546,   -0.5266678),
vec2( 0.8030878,    0.4266291),
vec2( 0.8259325,    0.08734058),
vec2( 0.8621388,   -0.3646038),
vec2( 0.9531851,    0.3011991),
vec2( 0.9578334,   -0.1584408),
vec2( 0.9898114,    0.1029227));

const vec2 Poisson128[128] = vec2[](
vec2(-0.9406119,    0.2160107),
vec2(-0.920003,     0.03135762),
vec2(-0.917876,    -0.2841548),
vec2(-0.9166079,   -0.1372365),
vec2(-0.8978907,   -0.4213504),
vec2(-0.8467999,    0.5201505),
vec2(-0.8261013,    0.3743192),
vec2(-0.7835162,    0.01432008),
vec2(-0.779963,     0.2161933),
vec2(-0.7719588,    0.6335353),
vec2(-0.7658782,   -0.3316436),
vec2(-0.7341912,   -0.5430729),
vec2(-0.6825727,   -0.1883408),
vec2(-0.6777467,    0.3313724),
vec2(-0.662191,     0.5155144),
vec2(-0.6569989,   -0.7000636),
vec2(-0.6021447,    0.7923283),
vec2(-0.5980815,   -0.5529259),
vec2(-0.5867089,    0.09857152),
vec2(-0.5774597,   -0.8154474),
vec2(-0.5767041,   -0.2656419),
vec2(-0.575091,    -0.4220052),
vec2(-0.5486979,   -0.09635002),
vec2(-0.5235587,    0.6594529),
vec2(-0.5170338,   -0.6636339),
vec2(-0.5114055,    0.4373561),
vec2(-0.4844725,    0.2985838),
vec2(-0.4803245,    0.8482798),
vec2(-0.4651957,   -0.5392771),
vec2(-0.4529685,    0.09942394),
vec2(-0.4523471,   -0.3125569),
vec2(-0.4268422,    0.5644538),
vec2(-0.4187512,   -0.8636028),
vec2(-0.4160798,   -0.0844868),
vec2(-0.3751733,    0.2196607),
vec2(-0.3656596,   -0.7324334),
vec2(-0.3286595,   -0.2012637),
vec2(-0.3147397,   -0.0006635741),
vec2(-0.3135846,    0.3636878),
vec2(-0.3042951,   -0.4983553),
vec2(-0.2974239,    0.7496996),
vec2(-0.2903037,    0.8890813),
vec2(-0.2878664,   -0.8622097),
vec2(-0.2588971,   -0.653879),
vec2(-0.2555692,    0.5041648),
vec2(-0.2553292,   -0.3389159),
vec2(-0.2401368,    0.2306108),
vec2(-0.2124457,   -0.09935001),
vec2(-0.1877905,    0.1098409),
vec2(-0.1559879,    0.3356432),
vec2(-0.1499449,    0.7487829),
vec2(-0.146661,    -0.9256138),
vec2(-0.1342774,    0.6185387),
vec2(-0.1224529,   -0.3887629),
vec2(-0.116467,     0.8827716),
vec2(-0.1157598,   -0.539999),
vec2(-0.09983152,  -0.2407187),
vec2(-0.09953719,  -0.78346),
vec2(-0.08604223,   0.4591112),
vec2(-0.02128129,   0.1551989),
vec2(-0.01478849,   0.6969455),
vec2(-0.01231739,  -0.6752576),
vec2(-0.005001599, -0.004027164),
vec2( 0.00248426,   0.567932),
vec2( 0.00335562,   0.3472346),
vec2( 0.009554717, -0.4025437),
vec2( 0.02231783,  -0.1349781),
vec2( 0.04694207,  -0.8347212),
vec2( 0.05412609,   0.9042216),
vec2( 0.05812819,  -0.9826952),
vec2( 0.1131321,   -0.619306),
vec2( 0.1170737,    0.6799788),
vec2( 0.1275105,    0.05326218),
vec2( 0.1393405,   -0.2149568),
vec2( 0.1457873,    0.1991508),
vec2( 0.1474208,    0.5443151),
vec2( 0.1497117,   -0.3899909),
vec2( 0.1923773,    0.3683496),
vec2( 0.2110928,   -0.7888536),
vec2( 0.2148235,    0.9586087),
vec2( 0.2152219,   -0.1084362),
vec2( 0.2189204,   -0.9644538),
vec2( 0.2220028,   -0.5058427),
vec2( 0.2251696,    0.779461),
vec2( 0.2585723,    0.01621339),
vec2( 0.2612841,   -0.2832426),
vec2( 0.2665483,   -0.6422054),
vec2( 0.2939872,    0.1673226),
vec2( 0.3235748,    0.5643662),
vec2( 0.3269232,    0.6984669),
vec2( 0.3425438,   -0.1783788),
vec2( 0.3672505,    0.4398117),
vec2( 0.3755714,   -0.8814359),
vec2( 0.379463,     0.2842356),
vec2( 0.3822978,   -0.381217),
vec2( 0.4057849,   -0.5227674),
vec2( 0.4168737,   -0.6936938),
vec2( 0.4202749,    0.8369391),
vec2( 0.4252189,    0.03818182),
vec2( 0.4445904,   -0.09360636),
vec2( 0.4684285,    0.5885228),
vec2( 0.4952184,   -0.2319764),
vec2( 0.5072351,    0.3683765),
vec2( 0.5136194,   -0.3944138),
vec2( 0.519893,     0.7157083),
vec2( 0.5277841,    0.1486474),
vec2( 0.5474944,   -0.7618791),
vec2( 0.5692734,    0.4852227),
vec2( 0.582229,    -0.5125455),
vec2( 0.583022,     0.008507785),
vec2( 0.6500257,    0.3473313),
vec2( 0.6621304,   -0.6280518),
vec2( 0.6674218,   -0.2260806),
vec2( 0.6741871,    0.6734863),
vec2( 0.6753459,    0.1119422),
vec2( 0.7083091,   -0.4393666),
vec2( 0.7106963,   -0.102099),
vec2( 0.7606754,    0.5743545),
vec2( 0.7846709,    0.2282225),
vec2( 0.7871446,    0.3891495),
vec2( 0.8071781,   -0.5257092),
vec2( 0.8230689,    0.002674922),
vec2( 0.8531976,   -0.3256475),
vec2( 0.8758298,   -0.1824844),
vec2( 0.8797691,    0.1284946),
vec2( 0.926309,     0.3576975),
vec2( 0.9608918,   -0.03495717),
vec2( 0.972032,     0.2271516));

// Using similar triangles from the surface point to the area light
float searchRegionRadiusUV(float z_world)
{
    return light_radius * (z_world - light_near_plane) / z_world;
}

// ------------------------------------------------------------------

// Using similar triangles between the area light, the blocking plane and the surface point
float penumbraRadiusUV(float z_receiver, float z_blocker)
{
    return light_radius * (z_receiver - z_blocker) / z_blocker;
}

// ------------------------------------------------------------------

// Project UV size to the near plane of the light
float projectToLightUV(float size_uv, float z_world)
{
    return size_uv * light_near_plane / z_world;
}

// ------------------------------------------------------------------

float zClipToEye(float z)
{
    return light_far_plane * light_near_plane / (light_far_plane - z * (light_far_plane - light_near_plane));
}

// ------------------------------------------------------------------

// Returns average blocker depth in the search region, as well as the number of found blockers.
// Blockers are defined as shadow-map samples between the surface point and the light.
void findBlocker(out float accum_blocker_depth,
out float num_blockers,
vec3 FragPos,
vec2      uv,
float     z0,
float     bias,
float     searchRegionRadiusUV,
uint      cascade_index)
{
    vec2 random_rotation = texture(gRandomAngles, FragPos * correction_factor).rg;

    accum_blocker_depth = 0.0;
    num_blockers        = 0.0;
    float biased_depth  = z0 - bias;

    for (int i = 0; i < u_blocker_search_samples; ++i)
    {
        vec2 offset;

        if (u_blocker_search_samples == 25)
        offset = Poisson25[i];
        else if (u_blocker_search_samples == 32)
        offset = Poisson32[i];
        else if (u_blocker_search_samples == 64)
        offset = Poisson64[i];
        else if (u_blocker_search_samples == 100)
        offset = Poisson100[i];
        else if (u_blocker_search_samples == 128)
        offset = Poisson128[i];

        // Add random rotation to the offset 
        offset = vec2(random_rotation.x * offset.x - random_rotation.y * offset.y,
        random_rotation.y * offset.x + random_rotation.x * offset.y);

        // Here use sampler without HW PCF filtering
        offset *= searchRegionRadiusUV;
        float shadow_map_depth = texture(gShadowMap, vec3(uv + offset, cascade_index)).r;

        if (shadow_map_depth < biased_depth)
        {
            accum_blocker_depth += shadow_map_depth;
            num_blockers++;
        }
    }
}

// ------------------------------------------------------------------

float shadowPCF(vec3 FragPos, vec2 uv, float z0, float bias, float filter_radius_uv, uint cascade_index)
{
    vec2 random_rotation = texture(gRandomAngles, FragPos * correction_factor).rg;

    float sum = 0.0;

    for (int i = 0; i < u_pcf_samples; ++i)
    {
        vec2 offset;

        if (u_pcf_samples == 25)
        offset = Poisson25[i];
        else if (u_pcf_samples == 32)
        offset = Poisson32[i];
        else if (u_pcf_samples == 64)
        offset = Poisson64[i];
        else if (u_pcf_samples == 100)
        offset = Poisson100[i];
        else if (u_pcf_samples == 128)
        offset = Poisson128[i];

        // Add random rotation to the offset 
        offset *= filter_radius_uv;
        offset = vec2(random_rotation.x * offset.x - random_rotation.y * offset.y,
        random_rotation.y * offset.x + random_rotation.x * offset.y);

        sum += texture(gShadowMapPCF, vec4(uv + offset, cascade_index, z0 - bias));
    }

    return sum / float(u_pcf_samples);
}

// ------------------------------------------------------------------

float shadowPCSS(vec3 FragPos, vec2 uv, float z, float bias, float z_vs, uint cascade_index)
{
    // ------------------------
    // STEP 1: blocker search
    // ------------------------
    float accum_blocker_depth, num_blockers;
    float searchRegionRadiusUV = searchRegionRadiusUV(z_vs);
    findBlocker(accum_blocker_depth, num_blockers,FragPos, uv, z, bias, searchRegionRadiusUV, cascade_index);

    if (num_blockers == 0.0)
    {
        return 1.0;
    }

    // ------------------------
    // STEP 2: penumbra size
    // ------------------------
    float avg_blocker_depth    = accum_blocker_depth / num_blockers;
    float avg_blocker_depth_vs = zClipToEye(avg_blocker_depth);
    float penumbra_radius      = penumbraRadiusUV(z_vs, avg_blocker_depth_vs);
    float filter_radius        = projectToLightUV(penumbra_radius, z_vs);

    // ------------------------
    // STEP 3: filtering
    // ------------------------
    return shadowPCF(FragPos, uv, z, bias, filter_radius, cascade_index);
}

// ------------------------------------------------------------------

float shadowOcclusionSoft(vec3 FragPos, vec3 Normal, uint cascade_index)
{
    vec3 proj_coords = out_pos_light_clip_space[cascade_index].xyz / out_pos_light_clip_space[cascade_index].w;

    // transform to [0,1] range
    proj_coords = proj_coords * 0.5 + 0.5;

    // get depth of current fragment from light's perspective
    float current_depth = proj_coords.z;
    if (current_depth > 1.0) return 1.0;

    // check whether current frag pos is in shadow
    float bias = max(0.001 * (1.0 - dot(normalize(Normal), -lights.directionalLight.direction)), 0.00001);

    vec4 pos_vs = out_pos_light_view_space[cascade_index];
    pos_vs.xyz /= pos_vs.w;

    light_near_plane = u_light_frustum_planes[cascade_index].x;
    light_far_plane  = u_light_frustum_planes[cascade_index].y;
    light_radius     = u_light_radius_uv / (pow(float(NUM_CASCADES), cascade_index) * float(NUM_CASCADES));

    return shadowPCSS(FragPos, proj_coords.xy, current_depth, bias, -(pos_vs.z), cascade_index);
}

// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / max(denom, 1e-5);
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / max(denom, 1e-5);
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);

    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}
// ----------------------------------------------------------------------------
vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

float GetSquareFalloffAttenuation(vec3 pos_to_light, float light_inv_radius)
{
    float distance_square = dot(pos_to_light, pos_to_light);
    float factor          = distance_square * light_inv_radius * light_inv_radius;
    float smooth_factor   = max(1.0 - factor * factor, 0.0);

    return (smooth_factor * smooth_factor) / max(distance_square, 1e-5);
}

vec3 CalculatePointLightPBR(PointLight light, vec3 FragPos, vec3 CamPos, vec3 Albedo, vec3 Normal, float Metallic, float Roughness, vec3 F0) {
    //---------------------------------------> View direction (V)
    vec3 V = normalize(CamPos - FragPos);
    vec3 L = normalize(light.position - FragPos);
    vec3 H = normalize(V + L); // Halfway
    //----------------------------------------> Fresnel
    vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);
    //----------------------------------------> Cook-Torrance BRDF
    float NDF = DistributionGGX(Normal, H, Roughness);
    float G   = GeometrySmith(Normal, V, L, Roughness);

    vec3 Nominator    = NDF * G * F;
    float Denominator = 4 * max(dot(Normal, V), 0.0) * max(dot(Normal, L), 0.0) + 0.001;
    vec3 Specular	  = Nominator / Denominator;

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - Metallic;
    
    float NDotL = max(dot(Normal, L), 0.0);
    vec3  light_direction = light.position - FragPos;
    float attenuation     = GetSquareFalloffAttenuation(light_direction, 1.0 / max(light.radius, 1e-5));
    vec3 Radiance = light.color * light.intensity;
    
    return ((kD * Albedo / PI + Specular) * Radiance * NDotL) * attenuation;
}

vec3 CalculateDirectionalLightPBR(DirectionalLight light, vec3 FragPos, vec3 CamPos, vec3 Albedo, vec3 Normal, float Metallic, float Roughness, vec3 F0) {
    //---------------------------------------> View direction (V)
    vec3 V = normalize(CamPos - FragPos);
    vec3 L = normalize(-light.direction);
    vec3 H = normalize(V + L); // Halfway
    //----------------------------------------> Fresnel
    vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);
    //----------------------------------------> Cook-Torrance BRDF
    float NDF = DistributionGGX(Normal, H, Roughness);
    float G   = GeometrySmith(Normal, V, L, Roughness);

    vec3 Nominator    = NDF * G * F;
    float Denominator = 4 * max(dot(Normal, V), 0.0) * max(dot(Normal, L), 0.0) + 0.001;
    vec3 Specular	  = Nominator / Denominator;

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - Metallic;

    float NDotL = max(dot(Normal, L), 0.0);
    vec3 Radiance = light.color * light.intensity;

    return (kD * Albedo / PI + Specular) * Radiance * NDotL;
}

vec3 CalculateIndirectLighting(vec3 FragPos, vec3 CamPos, vec3 Albedo, vec3 Normal, float Metallic, float Roughness, float AO) {
    //---------------------------------------> View direction (V)
    vec3 V = normalize(CamPos - FragPos);
    vec3 R = reflect(-V, Normal);
    //----------------------------------------> Specular reflectance at normal incidence
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, Albedo, Metallic);
    //----------------------------------------> Fresnel
    vec3 F = FresnelSchlickRoughness(max(dot(Normal, V), 0.0), F0, Roughness);
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - Metallic;
    //----------------------------------------> Diffuse IBL
    vec3 irradiance = texture(gIrradiance, Normal).rgb;
    vec3 diffuse      = irradiance * Albedo;
    //----------------------------------------> Specular IBL
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(gPrefilter, R, Roughness * MAX_REFLECTION_LOD).rgb;
    vec2 BRDF  = texture(gBRDFLut, vec2(max(dot(Normal, V), 0.0), Roughness)).rg;
    vec3 specular = prefilteredColor * (F * BRDF.x + BRDF.y);

    return (kD * diffuse * specular) * AO;
}

vec3 CalculateLo(vec3 L, vec3 N, vec3 V, vec3 Ra, vec3 F0, float R, float M, vec3 A)
{
    vec3 H = normalize(V + L); //Halfway Vector

    //Cook-Torrance BRDF
    float D = DistributionGGX(N, H, R);
    float G = GeometrySmith(N, V, L, R);
    vec3  F = FresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 Nominator    = D * G * F;
    float Denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 Specular	  = Nominator / Denominator;

    vec3 Ks = F;
    vec3 Kd = vec3(1.0) - Ks;
    Kd *= 1.0 - M;

    float NDotL = max(dot(N, L), 0.0);
    return (Kd * A / PI + Specular) * Ra * NDotL;
}

float shadow(vec3 FragPos, vec3 ViewPos, vec3 Normal) {
    /*
    for (int i = 0; i < NUM_CASCADES; ++i)
    {
        out_pos_light_view_space[i] = u_light_views[i] * vec4(FragPos, 1.0);
        out_pos_light_clip_space[i] = u_light_view_projections[i] * vec4(FragPos, 1.0);
    }
    */

    uint cascade_index = 0;

    for (uint i = 0; i < NUM_CASCADES - 1; ++i)
    {
        if (ViewPos.z < u_cascade_splits[i])
        {
            cascade_index = i + 1;
        }
    }
    float shadow_factor = 0.0;
    shadow_factor = shadowOcclusionSoft(FragPos, Normal, cascade_index);
    return shadow_factor;
}

void main() {
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 ViewPos = texture(gViewPosition, TexCoords).rgb;
    vec4 AlbedoSrc = texture(gAlbedo, TexCoords);
    vec3 Albedo = AlbedoSrc.rgb;
    float Alpha = AlbedoSrc.a;
    vec3 N = texture(gNormal, TexCoords).rgb;
    float Roughness = texture(gRoughMetalAO, TexCoords).r;
    float Metallic  = texture(gRoughMetalAO, TexCoords).g;
    float AO        = texture(gRoughMetalAO, TexCoords).b;
    vec3 CamPos = camera.u_Position;

    vec3 V = normalize(CamPos - FragPos);
    vec3 R = reflect(-V, N);
    
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, Albedo, Metallic);

    vec3 Lighting = vec3(0);
    Lighting += CalculateDirectionalLightPBR(lights.directionalLight, FragPos, CamPos, Albedo, N, Metallic, Roughness, F0);
    for (uint i = 0; i < 1; i++){
          //Lighting += CalculatePointLightPBR(lights.pointLight, FragPos, CamPos, Albedo, N, Metallic, Roughness, F0);
        vec3 L = normalize(lights.pointLight.position - FragPos);
        float distance = length(lights.pointLight.position - FragPos);
        float attenuation = 1.0/(distance * distance);
        vec3 Ra = lights.pointLight.color * attenuation;
        Lighting += CalculateLo(L, N, V, Ra, F0, Roughness, Metallic, Albedo);
    }
    // Indirect IBL Lighting
    vec3 IndirectLighthing = CalculateIndirectLighting(FragPos, CamPos, Albedo, N, Metallic, Roughness, AO);

    vec3 F = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, Roughness);

    vec3 Ks = F;
    vec3 Kd = 1.0 - Ks;
    Kd *= 1.0 - Metallic;
    
    vec3 irradiance = texture(gIrradiance, N).rgb;
    vec3 diffuse    = irradiance * Albedo;

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(gPrefilter, R,  Roughness * MAX_REFLECTION_LOD).rgb;
    vec2 BRDF  = texture(gBRDFLut, vec2(max(dot(N, V), 0.0), Roughness)).rg;
    vec3 specular = prefilteredColor * (F * BRDF.x + BRDF.y);

    vec3 ambient = (Kd * diffuse + specular) * AO;
    
    float shadow = shadow(FragPos, ViewPos, N);
    vec3 result = vec3(0);
    result =  Lighting + ambient;
    result *= shadow;
    
    // HDR tonemapping
    result = result / (result + vec3(1.0));
    // gamma correct
    result = pow(result, vec3(1.0/2.2));

    FragColor = vec4(result, Alpha);
}