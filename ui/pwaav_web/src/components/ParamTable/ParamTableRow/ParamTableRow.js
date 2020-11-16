import React from 'react';
import {
    TableRow,
    TableCell,
} from '@material-ui/core';

import SetButton from '../../../elements/SetButton/SetButton'
import ParamEntryField from './ParamEntryField/ParamEntryField'

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
