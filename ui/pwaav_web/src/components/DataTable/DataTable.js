import React from 'react';
import {
    Table,
    TableBody,
    TableCell,
    TableContainer,
    TableHead,
    TableRow,
} from '@material-ui/core';

import DataTableRow from './DataTableRow/DataTableRow'

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
                        /* Generate a table row for each piece of data in the system */
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
