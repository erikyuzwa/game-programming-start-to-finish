uniform sampler2D sdl_tex;
varying vec2 texCoords;

void main( void )
{
	gl_FragColor = texture2D(sdl_tex, texCoords);

}
