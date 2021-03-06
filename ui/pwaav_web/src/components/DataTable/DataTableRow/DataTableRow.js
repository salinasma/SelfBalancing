import React from 'react';
import {
    TableRow,
    TableCell,
} from '@material-ui/core';

function DataTableRow(props) {
    return (
        <TableRow>
            <TableCell component="th" scope="row">{props.dataName}</TableCell>
            <TableCell>{props.dataVal}</TableCell>
        </TableRow>
    );
}

export default DataTableRow;
