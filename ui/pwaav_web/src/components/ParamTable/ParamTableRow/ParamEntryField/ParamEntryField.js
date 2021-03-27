import React from 'react';
import {
    TextField,
} from '@material-ui/core';


function ParamEntryField(props) {
    return (
        <TextField id={props.paramName} value={props.setParamVal} onChange={props.paramEntryHandler}/>
    );
}

export default ParamEntryField;
