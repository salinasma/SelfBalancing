import React from 'react';
import {
    Table,
    TableBody,
    TableCell,
    TableContainer,
    TableHead,
    TableRow,
} from '@material-ui/core';

import ParamTableRow from './ParamTableRow/ParamTableRow'

function ParamTable(props) {
    return (
        <TableContainer>
            <Table aria-label="Parameters Table" size="small">
                <TableHead>
                    <TableRow>
                        <TableCell>Parameter</TableCell>
                        <TableCell>Current Value</TableCell>
                        <TableCell>Set Value</TableCell>
                    </TableRow>
                </TableHead>
        
                <TableBody>
                    {   
                        Object.keys(props.paramReg).map(function(paramKey) {
                            return (
                                /* Generate a table row for each piece of data in the system */
                                <ParamTableRow paramName={paramKey} paramType={props.paramTypes[paramKey]} paramVal={props.paramReg[paramKey]} paramEntryHandler={props.paramEntryHandler} paramUpdateHandler={props.paramUpdateHandler} />
                            );
                        })
                    }
                </TableBody>
            </Table>
        </TableContainer>
    );
}

export default ParamTable;
