import React, { Component } from 'react';
import './App.css';
import {
    Button,
    TextField,
    Typography,
    Table,
    TableBody,
    TableCell,
    TableContainer,
    TableHead,
    TableRow,
} from '@material-ui/core';
import { 
    makeStyles, 
    withStyles 
} from '@material-ui/core/styles';

import DataTableRow from './DataTableRow'

function DataTable(props) {
    return (
        <TableContainer>
            <Table aria-label="Data Table" size="small">
                <TableHead>
                    <TableRow>
                        <TableCell>Data</TableCell>
                        <TableCell>Current Value</TableCell>
                    </TableRow>
                </TableHead>
        
                <TableBody>
                    {   
                        Object.keys(props.dataReg).map(function(dataKey) {
                            return (
                                <DataTableRow dataName={dataKey} dataVal={props.dataReg[dataKey]} />
                            );
                        })
                    }
                </TableBody>
            </Table>
        </TableContainer>
    );
}

export default DataTable;
