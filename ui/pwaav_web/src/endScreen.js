
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
  Text

} from '@material-ui/core';


class EndScreen extends React.Component {
    render() {
        return ( 
            <div style={{margin:10}} >

<div style={{alignItems: "center", justifyContent: 'center'}}>
        <Typography variant="h5"  color="textPrimary" component="h1"  style={{textAlign: 'center'}}  >
            Happy Birthday Vamsi!
            </Typography>
</div>
            <Typography  color="textPrimary" component="p" style={{textAlign: 'center'}}>
                put in some message here
            </Typography>
            </div>
            );

    }
}

export default EndScreen;