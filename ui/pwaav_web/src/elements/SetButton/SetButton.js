import React from 'react';
import { Button } from '@material-ui/core';

const SetButton = function(props) {
    return (
        <Button onClick={() => props.paramUpdateHandler(props.paramName)}>Set</Button>
    );
}

export default SetButton;
