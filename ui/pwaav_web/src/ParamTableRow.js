import React, { Component } from 'react';
import './App.css';
import {
    TableRow,
    TableCell,
    Typography,
    TextField,
} from '@material-ui/core';

import SetButton from './SetButton'
import ParamEntryField from './ParamEntryField'

function ParamTableRow(props) {
    return (
        <TableRow>
            <TableCell component="th" scope="row">
                {props.paramName}
            </TableCell>
            <TableCell>
                {props.paramVal}
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
