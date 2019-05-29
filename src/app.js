import React from 'react';
import ReactDom from 'react-dom';
import 'normalize.css/normalize.css';
import './styles/styles.scss';
import AppRouter from './router/AppRouter';

ReactDom.render(<AppRouter />, document.getElementById('app'));