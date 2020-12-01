export interface LightState {
  led_on: boolean;
}
export interface RGB {
  cmd?: number;
  mode?: number;
  modeString?: string;
  brightness?: number
  c1?: Color;
  c2?: Color;
  angle?: number;
  blur?: number;
  center?: number;
}
export interface Color {
  r: number;
  g: number;
  b: number;
}
export interface LightMqttSettings { 
  unique_id : string;
  name: string;
  mqtt_path : string;
}
