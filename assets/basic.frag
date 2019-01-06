precision mediump float;

uniform float time;
uniform vec2 resolution;
uniform vec2 mouse;

uniform sampler2D image0;

out vec4 fragColor;

void main(void) {
  vec2 p = gl_FragCoord.xy / resolution;

  vec3 rgb = texture(image0, p).rgb;

  fragColor = vec4(rgb, 1.0);
}
