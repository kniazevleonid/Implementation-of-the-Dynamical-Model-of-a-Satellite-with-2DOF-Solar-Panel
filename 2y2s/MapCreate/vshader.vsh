attribute highp vec4 a_position;
attribute highp vec2 a_textcoord0;
attribute highp vec3 a_normal;
uniform highp mat4 u_projectionMatrix;
uniform highp mat4 u_viewMatrix;
uniform highp mat4 u_modelMatrix;
varying highp vec4 v_position;
varying highp vec2 v_textcoord0;
varying highp vec3 v_normal;

uniform highp mat4 u_projectionLightMatrix;
uniform highp mat4 u_shadowLightMatrix;
uniform highp mat4 u_LightMatrix;
uniform highp vec4 u_lightDirection;
varying highp vec4 v_lightDirection;
varying highp vec4 v_positionLightMatrix;


void main(void)
{
    mat4 mv_matrix = u_viewMatrix * u_modelMatrix;

    gl_Position = u_projectionMatrix * mv_matrix * a_position;
    v_textcoord0 = a_textcoord0;
    v_normal = normalize(vec3(mv_matrix * vec4(a_normal, 0.0)));
    v_position = mv_matrix * a_position;

    v_positionLightMatrix = u_projectionLightMatrix * u_shadowLightMatrix * u_modelMatrix * a_position;;

    v_lightDirection = u_viewMatrix * u_LightMatrix * u_lightDirection;

}
