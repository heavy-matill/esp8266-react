import React, { Component } from 'react';
import { ValidatorForm, TextValidator } from 'react-material-ui-form-validator';

import { Typography, Box, Switch, Slider, rgbToHex, hexToRgb, Button } from '@material-ui/core';
import { decomposeColor } from '@material-ui/core/styles/colorManipulator';
import { Input } from '@material-ui/core';
import SaveIcon from '@material-ui/icons/Save';
import { WEB_SOCKET_ROOT } from '../api';
import { WebSocketControllerProps, WebSocketFormLoader, WebSocketFormProps, webSocketController, FormActions, FormButton,} from '../components';
import { SectionContent, BlockFormControlLabel } from '../components';

import { LightState, RGB, Color } from './types';
import { CheckBox, DateRange } from '@material-ui/icons';

export const RGB_WEBSOCKET_URL = WEB_SOCKET_ROOT + "rgbState";

type RGBWebSocketControllerProps = WebSocketControllerProps<RGB>; //`? type

/*function whiteColor(brightness: number) {
  let c: Color = { r: brightness, g: brightness, b: brightness }
  return c;
}*/
class RGBWebSocketController extends Component<RGBWebSocketControllerProps> {

  render() {
    return (
      <SectionContent title='WebSocket Controller' titleGutter>
        <WebSocketFormLoader
          {...this.props}
          render={props => (
            <RGBWebSocketControllerForm {...props} />
          )}
        />
      </SectionContent>
    )
  }

}

export default webSocketController(RGB_WEBSOCKET_URL, 100, RGBWebSocketController);

type RGBWebSocketControllerFormProps = WebSocketFormProps<RGB>;

function RGBWebSocketControllerForm(props: RGBWebSocketControllerFormProps) {
  const { data, saveData, setData } = props;

  const changeBrightness = (event: React.ChangeEvent<{}>, value: any) => {
    console.log("changeBrightness", event, value)
    data.mode = 0;
    data.mode = value;
    setData(data, saveData); // , c1: whiteColor(value)
  }
  const changeColor = (event: any, key?: String) => {
    console.log("changeColor", event, key)
    if (key === undefined)
      key = event.target.id;
    // change local style
    let color = event.target.value;
    //event.currentTarget.parentElement.style.backgroundColor = color;
    // send data
    let rgb = decomposeColor(color).values;
    let c: Color = { r: rgb[0], g: rgb[1], b: rgb[2] };
    if (key === "c2") {
      data.mode = 2;
      data.c2 = c;
    } else {
      data.mode = 1;
      data.c1 = c;
    }
    setData(data, saveData);
  }
  const save = (event: any) => {
    data.cmd=250;
    setData(data, saveData);
  }
  const load = (event: any) => {
    data.cmd=251;
    setData(data, saveData);
  }
  return (
    <ValidatorForm onSubmit={saveData}>
      <Box bgcolor="primary.main" color="primary.contrastText" p={2} mt={2} mb={2}>
        <Typography variant="body1">
          The switch below controls the LED via the WebSocket. It will automatically update whenever the LED state changes.
        </Typography>
      </Box>

      <Typography variant="body1">
        Brightness
        </Typography>
      <Slider
        max={255}
        defaultValue={data.brightness}
        valueLabelDisplay="auto"
        color="primary"
        onChange={changeBrightness}
      />

      <Typography variant="body1">
        Color Input
        </Typography>
      <Typography
        style={{
          backgroundColor:
            rgbToHex("rgb(" + [data.c1.r, data.c1.g, data.c1.b].join(',') + ")"),
        }}>
        <Input
          id="c1"
          type="color"
          fullWidth={true}
          value={
            rgbToHex("rgb(" + [data.c1.r, data.c1.g, data.c1.b].join(',') + ")")
          }
          onChange={(e) => changeColor(e, 'c1')}
        /></Typography>

      <Typography variant="body1">
        Color 2
        </Typography>
      <Typography
        style={{
          backgroundColor:
            rgbToHex("rgb(" + [data.c2.r, data.c2.g, data.c2.b].join(',') + ")"),
        }}>
        <Input
          id="c2"
          type="color"
          fullWidth={true}
          value={
            rgbToHex("rgb(" + [data.c2.r, data.c2.g, data.c2.b].join(',') + ")")
          }
          onChange={(e) => changeColor(e, 'c2')}
        /></Typography>

<Button startIcon={<SaveIcon />} variant="contained" color="primary" onClick={save}>
          Save
        </Button>
        <Button startIcon={<SaveIcon />} variant="contained" color="primary" onClick={load}>
          Load
        </Button>

    </ValidatorForm>
  );
}
