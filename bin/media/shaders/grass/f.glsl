uniform sampler2D Texture0;
varying vec2 TexCoord;

uniform float AmplitudeFactor;
uniform float YStretch;
uniform float Time;

void main() 
{
   vec2 tmp = TexCoord;
   tmp.x += sin( (gl_FragCoord.y + Time)/ YStretch) / AmplitudeFactor;

   vec4 col = tex2D(Texture0, tmp);
   gl_FragColor = col;
}
