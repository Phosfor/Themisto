uniform sampler2D Texture0;
varying vec2 TexCoord;

void main(void)
{
   //vec4 rgba = texture2D(Texture0, TexCoord);
   //gl_FragColor = rgba;

/////////////////////////// Bloom
/*
   vec4 sum = vec4(0);
   int j;
   int i;

   for( i= -4 ;i < 4; i++)
   {
        for (j = -3; j < 3; j++)
        {
            sum += texture2D(Texture0, TexCoord + vec2(j, i)*0.004) * 0.25;
        }
   }
       if (texture2D(Texture0, TexCoord).r < 0.3)
    {
       gl_FragColor = sum*sum*0.012 + texture2D(Texture0, TexCoord);
    }
    else
    {
        if (texture2D(Texture0, TexCoord).r < 0.5)
        {
            gl_FragColor = sum*sum*0.009 + texture2D(Texture0, TexCoord);
        }
        else
        {
            gl_FragColor = sum*sum*0.0075 + texture2D(Texture0, TexCoord);
        }
    }
*/

/////////////////////// BLOOM 2
/*
    vec2 dx1   = vec2 ( 1.0 / 512.0, 0.0 );
    vec2 dy1   = vec2 ( 0.0, 1.0 / 512.0 );
    vec4 base = texture2D ( Texture0, TexCoord.xy ) +
                texture2D ( Texture0, TexCoord.xy + dx1 ) +
                texture2D ( Texture0, TexCoord.xy + dy1 ) +
                texture2D ( Texture0, TexCoord.xy + dx1 + dy1 );

    base *= 0.35;

    if ( length ( base.rgb ) < 0.8 )
        base.rgb = vec3 ( 0.0 );

  //gl_FragColor = base;
*/

/////////////////////////////////// BLUR horizontal
    vec2 tx  = TexCoord.xy;
    vec2 dx  = vec2 (0.001953,0.000000);
    vec2 sdx = dx;
    vec4 sum = texture2D ( Texture0, tx ) * 0.134598;

    sum += (texture2D ( Texture0, tx + sdx ) + texture2D ( Texture0, tx - sdx ) )* 0.127325;
    sdx += dx;
    sum += (texture2D ( Texture0, tx + sdx ) + texture2D ( Texture0, tx - sdx ) )* 0.107778;
    sdx += dx;
    sum += (texture2D ( Texture0, tx + sdx ) + texture2D ( Texture0, tx - sdx ) )* 0.081638;
    sdx += dx;
    sum += (texture2D ( Texture0, tx + sdx ) + texture2D ( Texture0, tx - sdx ) )* 0.055335;
    sdx += dx;
    sum += (texture2D ( Texture0, tx + sdx ) + texture2D ( Texture0, tx - sdx ) )* 0.033562;
    sdx += dx;
    sum += (texture2D ( Texture0, tx + sdx ) + texture2D ( Texture0, tx - sdx ) )* 0.018216;
    sdx += dx;
    sum += (texture2D ( Texture0, tx + sdx ) + texture2D ( Texture0, tx - sdx ) )* 0.008847;
    sdx += dx;

    sum.a = 0.4f;

    gl_FragColor = sum;

    //gl_FragColor.rgb = vec3(1.0f, 0.0f, 0.0f);
    //gl_FragColor.a = 1.0f;
}
