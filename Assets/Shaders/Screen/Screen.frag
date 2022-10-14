#version 460
layout (location = 0) out vec4 FragColor;
layout (location = 0) in vec2 vTexCoord;
layout (binding = 0) uniform sampler2D uTexture;

const int blurring_mat_small_size = 3;
float blurring_mat_small[blurring_mat_small_size] = float[](1, 2, 1);
float blurring_mat_small_coef = 1.0 / 16.0;

const int blurring_mat_med_size = 5;
float blurring_mat_med[blurring_mat_med_size] = float[](1, 4, 6, 4, 1);
float blurring_mat_med_coef = 1.0 / 256.0;

float col_to_gs(vec3 vec) {
    return 0.21 * vec.x + 0.72 * vec.y + 0.07 * vec.z;
}

void main()
{
/*
    const int sobel_x_size = 3;
    float sobel_x[sobel_x_size * sobel_x_size] = float[](1, 0, -1, 4, 0, -4, 1, 1, -1);

    const int sobel_y_size = 3;
    float sobel_y[sobel_y_size * sobel_y_size] = float[](4, 2, 1, 2, 0, 0, -3, -2, -1);


    float edge_x = 0.0;
    float edge_y = 0.0;

    for (int i = 0; i < sobel_x_size; i++) {
        float rel_i = i - (sobel_x_size - 1.0) / 2.0;

        for (int j = 0; j < sobel_y_size; j++) {
            float rel_j = j - (sobel_y_size - 1.0) / 2.0;
            float grayscale_pixel = col_to_gs(texture(uTexture, vTexCoord + vec2(rel_i / 1920, rel_j / 1080)).rgb);
            edge_x += sobel_x[j * 3 + i] * grayscale_pixel;
            edge_y += sobel_y[j * 3 + i] * grayscale_pixel;
        }
    }

    FragColor = vec4(vec3(sqrt(edge_x * edge_x + edge_y * edge_y)), 1.0);
*/
    FragColor = vec4(texture(uTexture, vTexCoord).rgb, 1.0);
}