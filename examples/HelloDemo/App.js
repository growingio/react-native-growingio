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
  {title: '标题', year: '2015', posters: {thumbnail: 'http://i.imgur.com/UePbdph.jpg'}},
];
const onButtonPress = () => {
    // Alert.alert('Button has been pressed!');
    NativeModules.GrowingIO.track("ggl", 
      100.00,
      "{\"ke1\": \"heel\",\"ke2\": 100}",
      (msg) => {
          if(msg == NativeModules.GrowingIO.CALLBACK_SUCCESS)
          {
            Alert.alert('SUCCESS');
          }else{
            Alert.alert(msg);
          }
        }
      );
};

export default class GIORNDemo extends Component {
   constructor(props) {
    super(props);
    this.state = {text: ''};
  }
  render() {
    let pic = {
      uri: 'https://upload.wikimedia.org/wikipedia/commons/d/de/Bananavarieties.jpg'
    };

    return (
      <View style={styles.container}>
        <Image source={{uri: 'asset:/lyasset.jpg'}} style={{width: 200, height: 110}} />
        <Image 
          source={require('./img/ipic.png')} style={{width: 193, height: 110}} />

        <TextInput
          style={{width: 200,height: 40}}
          placeholder="Type here to translate!"
          growingParams = {{"track":"true"}}
          onChangeText={(text) => this.setState({text})}
        />

        <Text style={{padding: 10, fontSize: 42}}>
          {this.state.text.split(' ').map((word) => word && '🍕').join(' ')}
        </Text>

        <Text 
          onPress={onButtonPress}
          style={styles.welcome}>
          下午好：）
        </Text>

        <Text style={styles.instructions}>
          To get started, edit index.android.js
        </Text>

        <Text style={styles.instructions}>
          Double tap R on your keyboard to reload,{'\n'}
          Shake or press menu button for dev menu
        </Text>

        <Button
          onPress={onButtonPress}
          title="Press Me"
          color="#841584"
          accessibilityLabel="See an informative alert"
        />

      <TouchableOpacity 
            onPress={onButtonPress}
            disabled={this.state.text.replace(/[ ]/g, "").length < 10}>
            <Text style={styles.instructions}>去登录</Text>
      </TouchableOpacity>

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
