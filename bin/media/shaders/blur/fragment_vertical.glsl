uniform sampler2D Texture0;
varying vec2 TexCoord;

void main (void)
{
   vec2 tx  = TexCoord.xy;
   vec2 dx  = vec2 (0.000000,0.001953);
   vec2 sdx = dx;
   vec4 sum = texture2D ( Texture0, tx ) * 0.065955;

   sum += (texture2D ( Texture0, tx + sdx ) + texture2D ( Texture0, tx - sdx ) )* 0.065626; 
   sdx += dx;
   sum += (texture2D ( Texture0, tx + sdx ) + texture2D ( Texture0, tx - sdx ) )* 0.064649; 
   sdx += dx;
   sum += (texture2D ( Texture0, tx + sdx ) + texture2D ( Texture0, tx - sdx ) )* 0.063053; 
   sdx += dx;
   sum += (texture2D ( Texture0, tx + sdx ) + texture2D ( Texture0, tx - sdx ) )* 0.060884; 
   sdx += dx;
   sum += (texture2D ( Texture0, tx + sdx ) + texture2D ( Texture0, tx - sdx ) )* 0.058205; 
   sdx += dx;
   sum += (texture2D ( Texture0, tx + sdx ) + texture2D ( Texture0, tx - sdx ) )* 0.055090; 
   sdx += dx;
   sum += (texture2D ( Texture0, tx + sdx ) + texture2D ( Texture0, tx - sdx ) )* 0.051623; 
   sdx += dx;
   sum += (texture2D ( Texture0, tx + sdx ) + texture2D ( Texture0, tx - sdx ) )* 0.047893; 
   sdx += dx;
   sum.a = 0.4;
   gl_FragColor = sum;
}
