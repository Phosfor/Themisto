attribute vec4 Position;
//attribute float HueOffset0;
attribute vec2 TexCoord0;

//varying float HueOffset;

uniform mat4 cl_ModelViewProjectionMatrix;

varying vec4 Color;
varying vec2 TexCoord;

void main(void)
{
    gl_Position = cl_ModelViewProjectionMatrix*Position;
  //  HueOffset = HueOffset0;
    TexCoord = TexCoord0;
}
