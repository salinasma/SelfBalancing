import React from 'react';
import {
    // Button,
    // TextField,
    Card,
    Grid,
    CardContent,
    // CardMedia,
    Typography,
    // CardActions,

} from '@material-ui/core';

class Miso extends React.Component {

    constructor(props) {
        super(props);

    }

    render() {
        return (
            <Grid item xl={3} l={3} xs={12} >
                {/* <Card style={cardStyle}> */}
                <Card >
                    <CardContent>
                        <Typography variant="h5" component="h1" color='primary' >MISO</Typography>
                    </CardContent>
                </Card>
            </Grid>

        );
    }


}

export default Miso;

