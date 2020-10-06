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

} from '@material-ui/core';


class Q3 extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      search: "Black",
      searchRows: [],
    };

    this.handleSubmit = this.handleSubmit.bind(this);
  }

  async handleSubmit(event) {
    event.preventDefault();
    var res = await fetch('http://127.0.0.1:8000/', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        "function": "question",
        "args": {
          "questionID": this.props.questionID,
          "searchString":
            this.state.search,
        }
      })
    });
    var response = await res.json();
    this.setState({ searchRows: response });

    console.log(response);
  }

  render() {

    var rows = [];

    const cardStyle = {
      maxWidth: 345,
      margin: 5


    }

    if (this.state.searchRows !== undefined)
      rows = this.state.searchRows.map(
        (row) => {

          return (
            // <Card style={{ maxWidth: 345, margin-top: 5, margin-left: 5}} >
            <Grid item xs={12} xl={2}>
              <Card style={cardStyle} >
                <CardMedia
                  wide
                  title={row[1]}
                  image={row[4]}
                  style={{ height: 140, }} >
                </CardMedia>
                <CardContent>
                  <Typography gutterBottom variant="h5" component="h2" >
                    {row[1]}

                  </Typography>
                  <Typography variant="body2" color="textSecondary" component="p" >
                    {row[3]}

                  </Typography>

                </CardContent>

                <CardActions style={{ justifyContent: 'space-between' }} >
                  <Typography gutterBottom variant="body2" component="textSecondary" >
                    ${row[2]}.00
                </Typography>
                  <Button size="small" color="primary">
                  </Button>
                  <Button size="small" color="primary">
                    Add to Cart
        </Button>
                </CardActions>

              </Card>

            </Grid>
          )
        });


    return (
      <div 
    //   className="App-body"  
      >

        <form onSubmit={this.handleSubmit}>
          <TextField

            id="searchField"
            label="search"
            variant="outlined"
            value={this.state.search}
            onChange={(event) => {
              // console.log(event.target.value) ;
              this.setState({ search: event.target.value })
            }}
          > </TextField >

          < Button
            color="secondary"
            onClick={this.handleSubmit}

          >Search</Button>
        </form>
        <Grid container spacing={1}>
          <Grid item xs={1}></Grid>
          {rows}
          <Grid item xs={1}></Grid>
        </Grid>



      </div>
    );

  }
}
export default Q3;