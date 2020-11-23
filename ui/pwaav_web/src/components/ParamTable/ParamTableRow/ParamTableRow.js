import React from 'react';
import {
    TableRow,
    TableCell,
} from '@material-ui/core';

import SetButton from '../../../elements/SetButton/SetButton'
import ParamEntryField from './ParamEntryField/ParamEntryField'

function ParamTableRow(props) {
    var displayVal = 0;

    console.log(props.paramName, " ", props.paramType, " ", props.paramVal);
    switch (props.paramType) {
        // INT
        case 0:
            displayVal = props.paramVal;
            break;
        // DOUBLE
        case 1:
            displayVal = Number.parseFloat(props.paramVal) / 1000;
            displayVal = displayVal.toFixed(3)
            break;
        default:
            displayVal = -1;
    }

    return (
        <TableRow>
            <TableCell component="th" scope="row">
                {props.paramName}
            </TableCell>
            <TableCell>
                {displayVal}
            </TableCell>
            <TableCell>
                <ParamEntryField paramName={props.paramName} paramEntryHandler={props.paramEntryHandler} />
            </TableCell>
            <TableCell>
                <SetButton paramName={props.paramName} paramUpdateHandler={props.paramUpdateHandler} />
            </TableCell>
        </TableRow>
    );
}

export default ParamTableRow;
