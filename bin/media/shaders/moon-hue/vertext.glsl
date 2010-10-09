attribute vec4 Position;
//attribute float ColorOffset0;
attribute vec2 TexCoord0;
uniform mat4 cl_ModelViewProjectionMatrix;

varying float ColorOffset;
varying vec2 TexCoord;

void main(void)
{
    gl_Position = cl_ModelViewProjectionMatrix*Position;
//    ColorOffset = ColorOffset0;
    TexCoord = TexCoord0;
}
