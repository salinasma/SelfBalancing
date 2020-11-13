import React, { Component } from 'react';
import './App.css';
import {Button} from '@material-ui/core';

const RefreshButton = function(props) {
    return (
        <Button onClick={props.clickHandler}>Refresh</Button>
    );
}

export default RefreshButton;
