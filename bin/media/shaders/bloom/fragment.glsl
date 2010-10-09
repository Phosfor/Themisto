uniform sampler2D Texture0;
uniform float BloomSize;

varying vec2 TexCoord;

void main(void)
{
   if (texture2D(Texture0, TexCoord).a == 0.0f)
   {
      discard;
   }
    vec2 dx1   = vec2 ( 1.0 / 512.0, 0.0 );
    vec2 dy1   = vec2 ( 0.0, 1.0 / 512.0 );
    vec4 base = texture2D ( Texture0, TexCoord.xy ) +
                texture2D ( Texture0, TexCoord.xy + dx1 ) +
                texture2D ( Texture0, TexCoord.xy + dy1 ) +
                texture2D ( Texture0, TexCoord.xy + dx1 + dy1 );

//base *= 0.19;
    base *= BloomSize;

    if ( length ( base.rgb ) < 0.8 )
        base.rgb = vec3 ( 0.0 );

    gl_FragColor = base;
}
