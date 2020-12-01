import React, { Component } from 'react';
import { Redirect, Switch, RouteComponentProps } from 'react-router-dom'

import { Tabs, Tab } from '@material-ui/core';

import { PROJECT_PATH } from '../api';
import { MenuAppBar } from '../components';
import { AuthenticatedRoute } from '../authentication';

import RGBInformation from './RGBInformation';
import RGBWebSocketController from './RGBWebSocketController';

import LightStateWebSocketController from './LightStateWebSocketController';
import LightMqttSettingsController from './LightMqttSettingsController';

class RGBProject extends Component<RouteComponentProps> {

  handleTabChange = (event: React.ChangeEvent<{}>, path: string) => {
    this.props.history.push(path);
  };

  render() {
    return (
      <MenuAppBar sectionTitle="RGB Project">
        <Tabs value={this.props.match.url} onChange={this.handleTabChange} variant="fullWidth">
          <Tab value={`/${PROJECT_PATH}/rgb/inforgb/123`} label="RGB Info" />
          <Tab value={`/${PROJECT_PATH}/rgb/socket/single`} label="RGB" />
          <Tab value={`/${PROJECT_PATH}/rgb/socket/dual`} label="RGB Dual" />
          <Tab value={`/${PROJECT_PATH}/rgb/socket/text`} label="RGB Text" />
          <Tab value={`/${PROJECT_PATH}/rgb/socket/special`} label="RGB Special" />
          <Tab value={`/${PROJECT_PATH}/rgb/mqtt`} label="MQTT Controller" />
        </Tabs>
        <Switch>
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/rgb/inforgb/:modestring`} component={RGBInformation} />
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/rgb/socket/:modeString`} component={RGBWebSocketController} />
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/rgb/lsocket`} component={LightStateWebSocketController} />
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/rgb/mqtt`} component={LightMqttSettingsController} />
          <Redirect to={`/${PROJECT_PATH}/rgb/information`} />
        </Switch>
      </MenuAppBar>
    )
  }

}

export default RGBProject;
