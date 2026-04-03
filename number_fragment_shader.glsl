// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;

// Texture sampler
uniform sampler2D onetex;

// new
uniform int number;

void main()
{
    float up = float(number)/10 + uv_interp.x/10;
    vec2 new_uv = vec2(up, uv_interp.y);   
    // Sample texture
    vec4 color = texture2D(onetex, new_uv);

    
   
    // Assign color to fragment
    gl_FragColor = vec4(color.r, color.g, color.b, color.a);
    

    // Check for transparency
    if(color.a < 1.0)
    {
         discard;
    }
}