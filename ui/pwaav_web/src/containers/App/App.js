import React, { Component } from 'react';
import './App.css';
import {
    Grid,
    Card,
    CardContent,
    Typography,
} from '@material-ui/core';
import ParamTable from '../../components/ParamTable/ParamTable';
import DataTable from '../../components/DataTable/DataTable';
import RefreshButton from '../../elements/RefreshButton/RefreshButton';

class App extends Component {
    constructor(props) {
        super(props);
        this.state = {
            state: null,
            address: "",
            dataVals: {},       
            paramVals: {},       
            paramTypes: {},       
            setParamVals: {}
        };
    }

    async getData() {
        console.log(this.state.address);
        var res = await fetch(new URL(this.state.address), {
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
        console.log(this.state.address);
        var res = await fetch(new URL(this.state.address), {
            method: 'POST',
            headers: {'Content-Type': 'application/json'},
            body: JSON.stringify({"function": "uiGetParams"})
        });
        console.log(res);
        var response = await res.json();
        console.log(response);

        const newParamVals = { ...this.state.paramVals };
        console.log(newParamVals);
        const newParamTypes = { ...this.state.paramTypes };
        console.log(newParamTypes);
        Object.keys(response).forEach(function(key) {
            newParamVals[key] = response[key]['value'];
            newParamTypes[key] = parseInt(response[key]['dataType']);
        });

        this.setState({ 
            state: response,
            paramVals: newParamVals,
            paramTypes: newParamTypes,
        });
    }

    async updateParam(paramKey) {
        var paramVal = 0;

        switch (this.state.paramTypes[paramKey]) {
            // Int
            case 0:
                paramVal = this.state.setParamVals[paramKey];
                break;
                
            // Double
            case 1:
                paramVal = Math.trunc(parseFloat(this.state.setParamVals[paramKey]) * 1000);
                break;

            default:
                paramVal = -1;
                break;
        }
        console.log("paramVal ", paramVal);

        console.log(this.state.address);
        var res = await fetch(new URL(this.state.address), {
            method: 'POST',
            headers: {'Content-Type': 'application/json'},
            body: JSON.stringify({  "function": "uiUpdateParam",
                                    "name": paramKey,
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

    async paramEntryHandler(event) {
        const newSetParamVals = {...this.state.setParamVals};
        newSetParamVals[event.target.id] = event.target.value;
        console.log(newSetParamVals);

        this.setState({
            setParamVals: newSetParamVals
        });
    }

    render() {
        return (
            <div className="Robot Dashboard">
                <Grid container wrap="nowrap" spacing={1}>
                    <Grid item xs={12} xl={3} l={3}>
                        <Card>
                            <CardContent>
                                <Typography align="center" variant="h5" component="h1" color='primary' >Programmable Parameters</Typography>
                                <ParamTable 
                                    paramTypes={this.state.paramTypes} 
                                    paramReg={this.state.paramVals} 
                                    paramEntryHandler={this.paramEntryHandler.bind(this)} 
                                    paramUpdateHandler={this.updateParam.bind(this)}
                                />
                                <RefreshButton clickHandler={this.getParams.bind(this)} />
                            </CardContent>
                        </Card>
                    </Grid>

                    <Grid item xs={12} xl={3} l={3}>
                        <Card>
                            <CardContent>
                                <Typography align="center" variant="h5" component="h1" color='primary' >Data</Typography>
                                <DataTable dataReg={this.state.dataVals} />
                                <RefreshButton clickHandler={this.getData.bind(this)} />
                            </CardContent>
                        </Card>
                    </Grid>

                    <Grid item xs={12} xl={3} l={3}>
                        <Card>
                            <CardContent>
                                <Typography align="center" variant="h5" component="h1" color='primary' >Robot</Typography>
                            </CardContent>
                        </Card>
                    </Grid>

                    <Grid item xs={12} xl={3} l={3}>
                        <Card>
                            <CardContent>
                                <Typography align="center" variant="h5" component="h1" color='primary' >Graphs</Typography>
                            </CardContent>
                        </Card>
                    </Grid>
                </Grid>
            </div>
        );
    }
}

export default App;
