import React from 'react';
import {
  Button,
  TextField,
  Card,
  Grid,
  CardContent,
  CardMedia,
  Typography,
  CardActions,
  Switch

} from '@material-ui/core';

import Miso from "./miso";
// import Mosi from "./mosi";




import { makeStyles, withStyles } from '@material-ui/core/styles';
import './App.css';

const useStyles = theme => ({
  root: {
    maxWidth: 345,
  },
  media: {
    height: 140,
  },
});

const styleClasses = useStyles();




function App() {
  return (
    <div className="App App-body">
      <StateDisplay></StateDisplay>
    </div>
  );

}

export default withStyles(useStyles)(App);



// const localhostAddress=  "http:/127.0.0.1:8000/"
const networkAddress = "http:/192.168.1.52:8000/"
var address = networkAddress




class StateDisplay extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      state: null,
      runningMode: false,
    };

  }
  async updateState() {
    console.log(address)
    var res = await fetch(new URL(address), {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        "function": "get_params",
        "args": {
          "searchString":
            this.state.search,
        }
      })
    });
    console.log(res);
    var response = await res.json();
    console.log(response);
    this.setState({ state: response });

    console.log(response);
  }
  async setMOSI() {
    console.log (this.state.runningMode)
    var res = await fetch(new URL(address), {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        "function": "set_mosi",
        "args": {
          "running": ! (this.state.runningMode)
        } 
      })
    });
    console.log(res);
    var response = await res.json();
    console.log(response);
    this.setState({ state: response });

    console.log(response);
  }



  render() {

    const cardStyle = {
      maxWidth: 345,
      margin: 5,
      flexGrow: 1,
    }

    const gridStyle = {
      flexGrow: 1
    }

    const switchStateChange = (event) => {
      this.setState(
        {
          // ...this.state.state,
          runningMode: event.target.checked 
        }
      )
      console.log(this.state.runningMode);
      this.setMOSI()
    }

    return (
      <div className="App App-body" style={gridStyle}>
        < Button
          onClick={(event) => { this.updateState() }}
        >Update</Button>
        <Grid container wrap="nowrap" spacing={1} >


          <Miso></Miso>
          {/* <Mosi></Mosi>  */}
          <Grid item xs={12} xl={3} l={3} flexGrow>
            <Card style={cardStyle}>
              <CardContent>
                <Typography variant="h5" component="h1" color='primary' >MOSI</Typography>
                <div style={{ display: "inline" }}>

                  <Typography variant="body1" component="h6"  >MOSI</Typography>
                  <Switch checked={this.state.runningMode}  onChange={ switchStateChange} ></Switch>
                </div>

              </CardContent>
            </Card>
          </Grid>

          <Grid item xs={12} xl={3} l={3} >
            <Card style={cardStyle}>
              <CardContent>
                <Typography variant="h5" component="h1" color='primary' >Param</Typography>
              </CardContent>
            </Card>
          </Grid>
        </Grid>
        <Typography variant="h5" component="h1" color='primary' >{JSON.stringify(this.state.state)}</Typography>
        <Typography variant="h5" component="h1" color='primary' >{JSON.stringify(this.state.runningMode)}</Typography>
      </div>);
  }

}


class DisplayDB extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      l: [],
    };
  }

  async getDBInfo(event) {
    event.preventDefault();
    var res = await fetch('http://192.168.1.75:8000/', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        "function": "print",
        "args": "",
      })
    });
    debugger;
    var response = await res.json();
    var output = [];
    response.forEach(function (entry) {
      console.log(entry)
      output.push(<div> {
        "\n\nCounter: " + entry[0] +
        "\nTime: " + entry[1] +
        "\nUsername: " + entry[2] +
        "\nVector: " + entry[3] +
        "\nMaterial: " + entry[4]
      } </div>);

      ;
    });
    this.setState({ l: output });
  }

  render() {
    return (
      <div className="App-body"  >

        <form onSubmit={this.handleSubmit}>
          <label> Input Vector form </label>
          < button onClick={(event) => { this.getDBInfo(event) }}>  Show DB Info</button>
          <br></br>
          <label>{this.state.l}</label>

        </form>

      </div>
    );

  }


}