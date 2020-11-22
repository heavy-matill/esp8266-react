import React, { Component } from 'react';
import { Typography, Box, List, ListItem, ListItemText, Slider } from '@material-ui/core';
import { SectionContent } from '../components';
import { WEB_SOCKET_ROOT } from '../api';
export const RGB_WEBSOCKET_URL = WEB_SOCKET_ROOT + "rgb";

type RGB = {
  mode?: number;
  brightness: number;
  colString2?: string;
}

class RGBInformation extends Component<{}, RGB> {
  
  constructor(props: RGB) {
    super(props);
    this.state = {brightness: 255};
    this.handleChange = this.handleChange.bind(this);
    this.handleSubmit = this.handleSubmit.bind(this);

  }
  handleChange(event: any, value: any) {
    this.setState({brightness: value});
  }

  handleSubmit(event: any) {
    alert('A name was submitted: ' + this.state.brightness); //error here
    event.preventDefault();
  }

  render() {
    return (
      <SectionContent title='RGB Information' titleGutter>
        <Typography variant="body1" paragraph>
          This is the RGB Light project.
        </Typography>
        <Typography variant="body1" paragraph>
          You can control the light.
        </Typography>

        <Typography variant="body1">
          Brightness: {this.state.brightness}
  </Typography>
        <Slider
          max={255}
          value={this.state.brightness}
          valueLabelDisplay="auto"
          color="primary"
          onChange={this.handleChange}
        />
        <Slider
          max={255}
          value={this.state.brightness}
          valueLabelDisplay="auto"
          color="primary"
          onChange={this.handleChange}
        />

      </SectionContent>
    )
  }

}

export default RGBInformation;
