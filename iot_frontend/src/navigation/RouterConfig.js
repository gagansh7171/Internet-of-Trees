import React from "react";
import { Route, Switch, Redirect } from "react-router-dom";
import  NotFound  from "./NotFound";
import Page from "../pages/page";

export const RouterConfig = () => {
  return (
      <Switch>
        {/* List all public routes here */}  
        <Route exact path="/fire" render={() => <Page title="Fire Threat"/>} />
        <Route exact path="/chop" render={() => <Page title="Chop Threat"/>} />
        <Route exact path="/fall" render={() => <Page title="Fall Threat" />} />
        <Route exact path="/fallen"  render={() => <Page title="Fallen"/>} />
        <Route exact path="/404" component={NotFound} />
        <Route exact path="/"  render={() => <Page title="All Trees"/>} />
        <Route exact path="">
          <Redirect to="/404" />
        </Route>
      </Switch>
  );
};