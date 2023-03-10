import React from "react";
import {connect} from 'react-redux';
import { withStyles } from "@material-ui/core/styles";
import Box from '@material-ui/core/Box';
import Typography from '@material-ui/core/Typography';

const useStyles = (theme) => ({
  error : {
    display:"flex",
    flexDirection:"column",
    alignItems:"center"
  },
});

class NotFound extends React.Component{
  constructor(props){
    super(props);
  }
  render(){
    const { classes } = this.props;
    return(
      <Box className={classes.error} mt={5} mb={5}>
        <img className="vert-move" src={this.props.image} height={200} />
        <br/><br/>
        <Typography variant="h3" ><Box fontFamily="Helvetica" fontWeight={800}>FOUNDN'T!</Box></Typography>
        <Typography align ="center">
          <Box maxWidth={470} mt={2} fontFamily="Helvetica" fontSize={17} fontWeight={500} >OOPS! Looks like the page you are looking for got lost in the stack. So either you need 
          a little typing lesson or I got some work to do. 
          Feel free to hit me @<a href="https://github.com/gagansh7171" nostyle><strong>gagansh7171</strong></a>.</Box>
        </Typography>
      </Box>
    
    );
  }
}


function mapStateToProps(state){
  return {
    currentTheme : state.app.theme || "light",
    image: ((theme) => {
      switch (theme) {
        case "light":
          return "./assets/images/search-stack-light.svg";
        case "dark":
          return "./assets/images/search-stack-dark.svg";
        default :
          return "./assets/images/search-stack-light.svg";
      }
    })(state.app.theme),
  };
}

function mapDispatchToProps(dispatch){
  return{
    toggleTheme: (t) => {
      let set;
      if(t=="light"){ set="dark";}
      else if(t=="dark"){ set="light";}
      return dispatch({type:"SET_THEME", theme : set});}
  };
}
export default withStyles(useStyles)(connect(mapStateToProps, mapDispatchToProps)(NotFound));