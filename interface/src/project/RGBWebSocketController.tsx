import React, { Component } from 'react';
import { RouteComponentProps, useParams } from 'react-router';
import { ValidatorForm, TextValidator } from 'react-material-ui-form-validator';

import { Typography, Box, Switch, Slider, rgbToHex, hexToRgb, Button } from '@material-ui/core';
import { decomposeColor } from '@material-ui/core/styles/colorManipulator';
import { Input } from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';
import { WEB_SOCKET_ROOT } from '../api';
import { WebSocketControllerProps, WebSocketFormLoader, WebSocketFormProps, webSocketController, FormActions, FormButton, } from '../components';
import { SectionContent, BlockFormControlLabel } from '../components';

import { LightState, RGB, Color } from './types';
import { CheckBox, DateRange } from '@material-ui/icons';
import RGBColor2 from './RGBColor2'

export const RGB_WEBSOCKET_URL = WEB_SOCKET_ROOT + "rgbState";

type RGBRouteProps = {
  modeString: string;
}
type RGBWebSocketControllerProps = WebSocketControllerProps<RGB>; //`? type

class RGBWebSocketController extends Component<RGBWebSocketControllerProps & RouteComponentProps<RGBRouteProps>> {
  render() {
    return (
      <SectionContent title='WebSocket Controller' titleGutter>
        {this.props.match.params.modeString == "single" && this.props.data != undefined && <WebSocketFormLoader {...this.props} render={props => (
          <RGBWebSocketControllerFormSingle {...props} />
        )} />}
        {this.props.match.params.modeString == "dual" && <WebSocketFormLoader {...this.props} render={props => (
          <RGBWebSocketControllerFormDual {...props} />
        )} />}
        {this.props.match.params.modeString == "text" && <WebSocketFormLoader {...this.props} render={props => (
          <RGBWebSocketControllerFormText {...props} />
        )} />}
        {this.props.match.params.modeString == "rainbow" && <WebSocketFormLoader {...this.props} render={props => (
          <RGBWebSocketControllerFormRainbow {...props} />
        )} />}
        {this.props.match.params.modeString == "special" && <WebSocketFormLoader {...this.props} render={props => (
          <RGBWebSocketControllerFormSpecial {...props} />
        )} />}
        {this.props.match.params.modeString == "animation" && <WebSocketFormLoader {...this.props} render={props => (
          <RGBWebSocketControllerFormAnimation {...props} />
        )} />}
      </SectionContent>
    )
  }
}

export default webSocketController(RGB_WEBSOCKET_URL, 100, RGBWebSocketController);

type RGBWebSocketControllerFormProps = WebSocketFormProps<RGB>;

function RGBWebSocketControllerFormMode(props: RGBWebSocketControllerFormProps) {
  const { data, saveData, setData } = props;
  const changeMode = (event: React.ChangeEvent<{}>, value: any) => {
    setData({...data, mode: value });
  }
  return (
    <ValidatorForm onSubmit={saveData}>
      <Typography variant="body1">
        Mode
        </Typography>
      <Slider
        max={5}
        defaultValue={data.mode}
        valueLabelDisplay="auto"
        color="primary"
        onChange={changeMode}
      />
    </ValidatorForm>
  );
}

function RGBWebSocketControllerFormSingle(props: RGBWebSocketControllerFormProps) {
  props.data.mode = 1;
  const { data, saveData, setData } = props;
  return (
    <div>
      <RGBWebSocketControllerFormColor1 {...props} />
      <RGBWebSocketControllerFormSpeedColor {...props} />
    </div>
  );
}

function RGBWebSocketControllerFormDual(props: RGBWebSocketControllerFormProps) {
  console.log("Dual mode = 4")
  props.data.mode = 4;
  const { data, saveData, setData } = props;
  return (
    <div>
      <RGBWebSocketControllerFormColor1 {...props} />
      <RGBWebSocketControllerFormColor2 {...props} />
      <RGBWebSocketControllerFormGradient {...props} />
      <RGBWebSocketControllerFormSpeedAngle {...props} />
      <RGBWebSocketControllerFormSpeedColor {...props} />
    </div>
  );
}

function RGBWebSocketControllerFormText(props: RGBWebSocketControllerFormProps) {
  return (
    <div>
      <RGBWebSocketControllerFormColor1 {...props} />
      <RGBWebSocketControllerFormColor2 {...props} />
    </div>
  );
}
function RGBWebSocketControllerFormRainbow(props: RGBWebSocketControllerFormProps) {
  console.log("Rainbow mode = 5")
  props.data.mode = 5;
  const { data, saveData, setData } = props;
  return (
    <div>
      <RGBWebSocketControllerFormMode {...props} />
      <RGBWebSocketControllerFormGradient {...props} />
      <RGBWebSocketControllerFormSpeedAngle {...props} />
      <RGBWebSocketControllerFormSpeedColor {...props} />
      <RGBWebSocketControllerFormButtons {...props} />
    </div>
  );
}
function RGBWebSocketControllerFormSpecial(props: RGBWebSocketControllerFormProps) {
  console.log("Special mode = 6")
  props.data.mode = 6;
  const { data, saveData, setData } = props;
  return (
    <div>
      <RGBWebSocketControllerFormMode {...props} />
      <RGBWebSocketControllerFormGradient {...props} />
      <RGBWebSocketControllerFormButtons {...props} />
    </div>
  );
}
function RGBWebSocketControllerFormAnimation(props: RGBWebSocketControllerFormProps) {
  const { data, saveData, setData } = props;
  return (
    <div>
      <RGBWebSocketControllerFormSpeedAngle {...props} />
    </div>
  );
}

function RGBWebSocketControllerFormColor1(props: RGBWebSocketControllerFormProps) {
  const { data, saveData, setData } = props;
  const changeColor1 = (event: any) => {
    let rgb = decomposeColor(event.target.value).values;
    setData({ ...data, c1: { r: rgb[0], g: rgb[1], b: rgb[2] } }, saveData);
  }
  return (
    <div>
      <Typography variant="body1">
        Color Input 1
        </Typography>
      <div
        style={{
          backgroundColor:
            "rgb(" + [data.c1!.r | 0, data.c1!.g | 0, data.c1!.b | 0].join(',') + ")",
        }}>
        <Input
          id="c1"
          type="color"
          value={
            rgbToHex("rgb(" + [data.c1!.r, data.c1!.g, data.c1!.b].join(',') + ")")
          }
          fullWidth={true}
          onChange={changeColor1}
        /></div>
    </div>
  );
}

function RGBWebSocketControllerFormColor2(props: RGBWebSocketControllerFormProps) {
  const { data, saveData, setData } = props;
  const changeColor2 = (event: any) => {
    let rgb = decomposeColor(event.target.value).values;
    setData({...data,  c2: { r: rgb[0], g: rgb[1], b: rgb[2] } }, saveData);
  }
  return (
    <ValidatorForm onSubmit={saveData}>
      <Typography variant="body1">
        Color Input 2
        </Typography>
      <div
        style={{
          backgroundColor:
            "rgb(" + [data.c2!.r, data.c2!.g, data.c2!.b].join(',') + ")",
        }}>
        <Input
          id="c2"
          type="color"
          value={
            rgbToHex("rgb(" + [data.c2!.r, data.c2!.g, data.c2!.b].join(',') + ")")
          }
          fullWidth={true}
          onChange={changeColor2}
        /></div>
    </ValidatorForm>
  );
}

function RGBWebSocketControllerFormGradient(props: RGBWebSocketControllerFormProps) {
  const { data, saveData, setData, handleValueChange } = props;

  const changeAngle = (event: React.ChangeEvent<{}>, value: any) => {
    if (value != data.angle)
      setData({ ...data, angle: value }, saveData);
  }
  const changeBlur = (event: React.ChangeEvent<{}>, value: any) => {
    if (value != data.blur)
      setData({...data,  blur: value }, saveData);
  }
  const changeCenter = (event: React.ChangeEvent<{}>, value: any) => {
    if (value != data.center)
      setData({ ...data, center: value }, saveData);
  }

  return (
    <div>
      <Typography variant="body1">
        Angle
        </Typography>
      <Slider
        max={360}
        value={data.angle}
        valueLabelDisplay="auto"
        color="primary"
        onChange={changeAngle}
        marks=
        {[{
          value: 0,
          label: '0°',
        }, {
          value: 45,
          label: '45°',
        }, {
          value: 90,
          label: '90°',
        }, {
          value: 135,
          label: '135°',
        }, {
          value: 180,
          label: '180°',
        }, {
          value: 225,
          label: '225°',
        }, {
          value: 270,
          label: '270°',
        }, {
          value: 315,
          label: '315°',
        }, {
          value: 360,
          label: '360°',
        }]}
      />
      <Typography variant="body1">
        Blur
        </Typography>
      <Slider
        max={100}
        value={data.blur}
        valueLabelDisplay="auto"
        color="primary"
        onChange={changeBlur}
        marks=
        {[{
          value: 0,
          label: '0%',
        }, {
          value: 50,
          label: '50%',
        }, {
          value: 100,
          label: '100%',
        }]}
      />
      <Typography variant="body1">
        Center
        </Typography>
      <Slider
        max={100}
        value={data.center}
        valueLabelDisplay="auto"
        color="primary"
        onChange={changeCenter}
        marks=
        {[{
          value: 0,
          label: '0%',
        }, {
          value: 50,
          label: '50%',
        }, {
          value: 100,
          label: '100%',
        }]}
      />
    </div>
  );
}
function RGBWebSocketControllerFormSpeedAngle(props: RGBWebSocketControllerFormProps) {
  const { data, saveData, setData, handleValueChange } = props;

  const changeSpeedAngle = (event: React.ChangeEvent<{}>, value: any) => {
    if (value != data.speed_angle)
      setData({...data,  speed_angle: value }, saveData);
  }

  return (
    <div>
      <Typography variant="body1">
        Angle animation speed
      </Typography>
      <Slider
        min={-100}
        max={100}
        value={data.speed_angle}
        valueLabelDisplay="auto"
        color="primary"
        onChange={changeSpeedAngle}
        marks=
        {[{
          value: 0,
          label: 'OFF',
        }]}
      />
    </div>
  );
}
function RGBWebSocketControllerFormSpeedColor(props: RGBWebSocketControllerFormProps) {
  const { data, saveData, setData, handleValueChange } = props;

  const changeSpeedColor = (event: React.ChangeEvent<{}>, value: any) => {
    if (value != data.speed_color)
      setData({...data,  speed_color: value }, saveData);
  }

  return (
    <div>
      <Typography variant="body1">
        Color animation speed
      </Typography>
      <Slider
        min={-100}
        max={100}
        value={data.speed_color}
        valueLabelDisplay="auto"
        color="primary"
        onChange={changeSpeedColor}
        marks=
        {[{
          value: 0,
          label: 'OFF',
        }]}
      />
    </div>
  );
}

function RGBWebSocketControllerFormButtons(props: RGBWebSocketControllerFormProps) {
  const { data, saveData, setData } = props;
  const save = (event: any) => {
    data.cmd = 250;
    setData(data, saveData);
  }
  const load = (event: any) => {
    data.cmd = 251;
    setData(data, saveData);
  }
  const animate = (event: any) => {
    data.cmd = 248;
    setData(data, saveData);
  }
  const doOther = (event: any) => {
    data.cmd = 249;
    setData(data, saveData);
  }
  return (
    <ValidatorForm onSubmit={saveData}>
      <Button startIcon={<SaveIcon />} variant="contained" color="primary" onClick={save}>
        Save
          </Button>
      <Button startIcon={<SaveIcon />} variant="contained" color="primary" onClick={load}>
        Load
          </Button>
      <Button startIcon={<SaveIcon />} variant="contained" color="primary" onClick={animate}>
        Animate
          </Button>
      <Button startIcon={<SaveIcon />} variant="contained" color="primary" onClick={doOther}>
        249
          </Button>
    </ValidatorForm>
  );
}
