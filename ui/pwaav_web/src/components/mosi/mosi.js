import React from 'react';
import {
    Button,
    TextField,
    Card,
    Grid,
    CardContent,
    CardMedia,
    Typography,
    CardActions,
    Switch

} from '@material-ui/core';

class Mosi extends React.Component {

    constructor(props) {
        super(props);

    }

    render() {
        return (
            <Grid item xs={12} xl={3} l={3}>
                <Card >
                {/* <Card style={cardStyle}> */}
                    <CardContent>
                        <Typography variant="h5" component="h1" color='primary' >MOSI</Typography>
                        <Switch  ></Switch>

                    </CardContent>
                </Card>
            </Grid>

        );
    }


}

export default Mosi;
