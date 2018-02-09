/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 * @flow
 */

import React, { Component } from 'react';
import App from './App';

import {
  AppRegistry,
  StyleSheet,
  Image,
  Text,
  View,
  TextInput,
  Button,
  TouchableOpacity,
  TouchableHighlight,
  TouchableNativeFeedback,
  TouchableWithoutFeedback,
  Alert,
  NativeModules
} from 'react-native';

var MOCKED_MOVIES_DATA = [
  {title: '标题', year: '2015'},
];

var trackObject = {title: '标题1', year: '201523'};
const track = () => {
    // Alert.alert('Button has been pressed!');
    NativeModules.GrowingIO.track("ggl", trackObject)
};

const trackWithNumber = () => {
    // Alert.alert('Button has been pressed!');
    NativeModules.GrowingIO.trackWithNumber("trackWithNumber", 10.12,trackObject)
};


const page = () => {
    // Alert.alert('Button has been pressed!');
    NativeModules.GrowingIO.page("GGLPage")
};

const setPageVariable = () => {
    // Alert.alert('Button has been pressed!');
    NativeModules.GrowingIO.setPageVariable("GGLPage",trackObject)
};

const setEvar = () => {
    // Alert.alert('Button has been pressed!');
    NativeModules.GrowingIO.setEvar(trackObject)
};

const setPeopleVariable = () => {
    // Alert.alert('Button has been pressed!');
    NativeModules.GrowingIO.setPeopleVariable(trackObject)
};

const serUserId = () => {
    // Alert.alert('Button has been pressed!');
    NativeModules.GrowingIO.serUserId("ggl")
};

const clearUserId = () => {
    // Alert.alert('Button has been pressed!');
    NativeModules.GrowingIO.clearUserId()
};

export default class GIORNDemo extends Component {
   constructor(props) {
    super(props);
    this.state = {text: ''};
  }
  render() {
    return (
      <View style={styles.container}>
        <Text 
          onPress={track}
          style={styles.welcome}>
          Track
        </Text>

        <Text 
          onPress={trackWithNumber}
          style={styles.welcome}>
          trackWithNumber
        </Text>

        <Text 
          onPress={setEvar}
          style={styles.welcome}>
          setEvar
        </Text>

        <Text 
          onPress={setPeopleVariable}
          style={styles.welcome}>
          setPeopleVariable
        </Text>

        <Text 
          onPress={page}
          style={styles.welcome}>
          Page
        </Text>

        <Text 
          onPress={setPageVariable}
          style={styles.welcome}>
          setPageVariable
        </Text>

        <Text 
          onPress={serUserId}
          style={styles.welcome}>
          serUserId
        </Text>

        <Text 
          onPress={clearUserId}
          style={styles.welcome}>
          clearUserId
        </Text>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#F5FCFF',
  },
  welcome: {
    fontSize: 20,
    textAlign: 'center',
    margin: 10,
  },
  instructions: {
    textAlign: 'center',
    color: '#333333',
    marginBottom: 5,
  },
});

AppRegistry.registerComponent('HelloDemo', () => App);
