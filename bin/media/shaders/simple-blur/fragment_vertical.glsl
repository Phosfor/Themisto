uniform sampler2D Texture0;
varying vec2 TexCoord;

#define	HALF_SIZE 3.0

void main (void)
{
   vec4 sum = vec4 ( 0 );
   vec2 dx  = vec2 ( 0, 4.0 / 512.0 );
   vec2 tx  = TexCoord - HALF_SIZE*dx;

   for ( float i = 0.0; i < (2.0*HALF_SIZE+1.0); i++ )
   {
      sum += clamp ( texture2D ( Texture0, tx ), 0.001, 10000.0 );
      tx  += dx;
   }

   gl_FragColor = clamp ( sum / (2.0*HALF_SIZE+1.0), 0.0, 10000.0 );
}
