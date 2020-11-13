import React, { Component } from 'react';
import './App.css';
import { Button } from '@material-ui/core';

const SetButton = function(props) {
    return (
        <Button onClick={() => props.paramUpdateHandler(props.paramName)}>Set</Button>
    );
}

export default SetButton;
