uniform sampler2D Texture0;
//varying float ColorOffset;
varying vec2 TexCoord;

void main(void)
{
   vec4 rgba = texture2D(Texture0, TexCoord);

//   rgba.r += ColorOffset;
//   rgba.r += 0.2f;

   rgba.a = max(rgba.a, 0.8f);
   rgba.rgb *= rgba.a;

   gl_FragColor = rgba;
}
