import React from 'react';
import { Router, Route, Switch, Link, NavLink } from 'react-router-dom';
import createHistory from 'history/createBrowserHistory';

import SMPage from '../components/SFPage';

// createHistory
export const history = createHistory();

const AppRouter = () => (
  <Router history={history}>
    <div>
    <Switch>
      <Route exact path="/" component={SMPage}/>
    </Switch>
    </div>
  </Router>
);


export default AppRouter;