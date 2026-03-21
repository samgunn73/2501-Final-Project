// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;

// for tiling
uniform vec2 tile_factor;

// Texture sampler
uniform sampler2D onetex;

void main()
{
    vec2 uv = uv_interp;

    // Scale UVs to create tiling
    vec2 tiled_uv = uv_interp * tile_factor;

    // Sample texture
    vec4 color = texture2D(onetex, tiled_uv);

    // Assign color to fragment
    gl_FragColor = vec4(color.r, color.g, color.b, color.a);

    // Check for transparency
    if(color.a < 1.0)
    {
         discard;
    }
}
