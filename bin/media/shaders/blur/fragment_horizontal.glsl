uniform sampler2D Texture0;
varying vec2 TexCoord;

void main(void)
{
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

/*
   if (texture2D(Texture0, TexCoord).a != 0.0f)
   {
      sum.a = 0.4;
   }
*/
//   sum.a = 0.9;

   gl_FragColor = sum;
}
