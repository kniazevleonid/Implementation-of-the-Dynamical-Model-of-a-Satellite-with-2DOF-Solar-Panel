uniform sampler2D u_texture;
uniform highp vec4 u_lightPosition;
uniform highp float u_lightPower;
varying highp vec4 v_position;
varying highp vec2 v_textcoord0;
varying highp vec3 v_normal;

void main(void)
{
    vec4 resultCol = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 eyePos = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 diffMatCol = texture2D(u_texture, v_textcoord0);
    vec3 eyeVec = normalize(v_position.xyz - eyePos.xyz);
    vec3 light =  normalize(v_position.xyz - u_lightPosition.xyz);
    vec3 reflectlight = normalize(reflect(light, v_normal));
    float len = length(v_position.xyz - eyePos.xyz);
    float specularFactor = 10.0;
    float ambientFactor = 0.1;

    vec4 diffcolor = diffMatCol * u_lightPower * max(0.0, dot(v_normal, -light)) / (1.0 + 0.25 * len * len);
    resultCol = diffcolor;
    vec4 ambient = ambientFactor * diffcolor;
    resultCol += ambient;
    vec4 specular = vec4(1.0, 1.0, 1.0, 1.0) * u_lightPower * pow(max(0.0, dot(reflectlight, -eyeVec)), specularFactor)/ (1.0 + 0.25 * len * len);

    resultCol += specular;

    gl_FragColor = resultCol;
    //gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
