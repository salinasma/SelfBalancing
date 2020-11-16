import React from 'react';
import './App.css';
import {
    Button,
    TextField,
    Card,
    Grid,
    CardContent,
    CardMedia,
    Typography,
    CardActions,
    Switch,
    List,
    ListItem,
    ListItemIcon,
    ListItemText,
    Table,
    TableBody,
    TableCell,
    TableContainer,
    TableHead,
    TableRow,
} from '@material-ui/core';
import { 
    makeStyles, 
    withStyles 
} from '@material-ui/core/styles';

import Miso from "./miso";
// import Mosi from "./mosi";

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
const networkAddress = "http:/192.168.1.17:8000/"

var address = networkAddress

class StateDisplay extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            state: null,
            dataVals: {"exampleData": 3},       
            paramVals: {"exampleData": 3}       
        };
    }

    async getData() {
        console.log(address);
        var res = await fetch(new URL(address), {
            method: 'POST',
            headers: {'Content-Type': 'application/json'},
            body: JSON.stringify({"function": "uiGetData"})
        });
        console.log(res);
        var response = await res.json();
        console.log(response);

        var newDataVals = { ...this.state.dataVals };
        Object.keys(response).forEach(function(key) {
            newDataVals[key] = response[key];
        });

        this.setState({ 
            state: response,
            dataVals: newDataVals
        });
    }

    async getParams() {
        console.log(address)
        var res = await fetch(new URL(address), {
            method: 'POST',
            headers: {'Content-Type': 'application/json'},
            body: JSON.stringify({"function": "uiGetParams"})
        });
        console.log(res);
        var response = await res.json();
        console.log(response);

        var newParamVals = { ...this.state.paramVals };
        Object.keys(response).forEach(function(key) {
            newParamVals[key] = response[key];
        });

        this.setState({ 
            state: response,
            paramVals: newParamVals
        });
    }

    async updateParam(paramName, paramVal) {
        console.log(address)
        var res = await fetch(new URL(address), {
            method: 'POST',
            headers: {'Content-Type': 'application/json'},
            body: JSON.stringify({  "function": "uiUpdateParam",
                                    "name": paramName,
                                    "value": paramVal
                                 })
        });
        console.log(res);
        var response = await res.json();
        console.log(response);

        this.setState({ 
            state: response,
        });
    }

    async txCmd() {
        console.log(address)
        var res = await fetch(new URL(address), {
            method: 'POST',
            headers: {'Content-Type': 'application/json'},
            body: JSON.stringify({  "function": "txCmd",
                                    "cmd": "cmdPlaceHolder",
                                 })
        });
        console.log(res);
        var response = await res.json();
        console.log(response);

        this.setState({ 
            state: response,
        });
    }

//    async setMOSI() {
//        console.log (this.state.runningMode)
//        var res = await fetch(new URL(address), {
//            method: 'POST',
//            headers: {
//                'Content-Type': 'application/json',
//            },
//            body: JSON.stringify({
//                "function": "set_mosi",
//                "args": {
//                    "running": ! (this.state.runningMode)
//                } 
//            })
//        });
//
//        console.log(res);
//        var response = await res.json();
//        console.log(response);
//        this.setState({ state: response });
//        
//        console.log(response);
//    }



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
                <Button onClick={(event) => { this.getData() }}> Update </Button>
                
                <Grid container wrap="nowrap" spacing={1}>
                    {/* <Miso></Miso> */}
                    {/* <Mosi></Mosi> */}

{/*            
                    <Grid item xs={12} xl={3} l={3} flexGrow>
                        <Card style={cardStyle}>
                            <CardContent>
                                <Typography variant="h5" component="h1" color='primary'>MOSI</Typography>
                                <div style={{ display: "inline" }}>
                                  <Typography variant="body1" component="h6">MOSI</Typography>
                                  <Switch checked={this.state.runningMode}  onChange={ switchStateChange} ></Switch>
                                </div>
                            </CardContent>
                        </Card>
                    </Grid>
*/}
                    
                    <Grid item xs={12} xl={3} l={3}>
                        <Card style={cardStyle}>
                            <CardContent>
                                <Typography variant="h5" component="h1" color='primary' >Param</Typography>
                                <List component="nav" aria-label="contacts">
                                    <ListItem divider>
                                        <ListItemText inset primary="Kp: " />
                                    </ListItem>
            
                                    <ListItem divider>
                                        <ListItemText inset primary="Ki: " />
                                    </ListItem>

                                    <ListItem divider>
                                        <ListItemText inset primary="Kd: " />
                                    </ListItem>
                                </List>
                            </CardContent>
                        </Card>
                    </Grid>

                    <Grid item xs={12} xl={3} l={3}>
                        <Card style={cardStyle}>
                            <CardContent>
                                <Typography variant="h5" component="h1" color='primary' >Robot</Typography>
                                <TableContainer>
                                    <Table aria-label="simple table">
                                        <TableHead>
                                            <TableRow>
                                                <TableCell>Parameter</TableCell>
                                                <TableCell align="right">Current Value</TableCell>
                                                <TableCell align="right">Set Value</TableCell>
                                            </TableRow>
                                        </TableHead>

                                        <TableBody>
                                            <TableRow>
                                                {
                                                    <TableCell component="th" scope="row">Gingivitis</TableCell>
                                                }   
                                              <TableCell align="right">Hello</TableCell>
                                              <TableCell align="right">Hello</TableCell>
                                            </TableRow>

                                            <TableRow>
                                              <TableCell component="th" scope="row">Gingivitis</TableCell>
                                              <TableCell align="right">Hello</TableCell>
                                              <TableCell align="right">Hello</TableCell>
                                            </TableRow>
                                        </TableBody>
                                    </Table>
                                </TableContainer>
                            </CardContent>
                        </Card>
                    </Grid>

                    <Grid item xs={12} xl={3} l={3}>
                        <Card style={cardStyle}>
                            <CardContent>
                                <Typography variant="h5" component="h1" color='primary' >Graphs</Typography>
                                <p>
                                <ol>
                                {
                                    Object.keys(this.state.dataVals).map(dataName => (
                                        <li>{dataName} = {this.state.dataVals[dataName]}</li>
                                    ))
                                }
                                </ol>
                                </p>
                            </CardContent>
                        </Card>
                    </Grid>

                    <Grid item xs={12} xl={3} l={3}>
                        <Card style={cardStyle}>
                            <CardContent>
                                <Typography variant="h5" component="h1" color='primary' >Input</Typography>
                                <TextField
                                      id="standard-number"
                                      label="Number"
                                      value={this.state.age}
                                      type="number"
                                      InputLabelProps={{
                                        shrink: true,
                                      }}
                                      margin="normal"
                                />
                            </CardContent>
                        </Card>
                    </Grid>
                </Grid>
                <Typography variant="h5" component="h1" color='primary' >{JSON.stringify(this.state.state)}</Typography>
                <Typography variant="h5" component="h1" color='primary' >{JSON.stringify(this.state.runningMode)}</Typography>
            </div>
        );
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
