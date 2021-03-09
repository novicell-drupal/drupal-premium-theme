const glob = require('glob');
const path = require('path');
const argv = require('minimist')(process.argv.slice(2));
const TerserPlugin = require('terser-webpack-plugin');
const BundleAnalyzer = require('webpack-bundle-analyzer');
const ESLintPlugin = require('eslint-webpack-plugin');

const mode = argv.mode || 'development';
const context = argv.context || '';

// Find all files JS from modules directory
/* eslint-disable */
const filesInModulesDir = glob.sync(process.env.fileMatch);
const allEntries = () => {
  const manyEntries = {};
  for (const index in filesInModulesDir) {
    let entry = path.basename(filesInModulesDir[index], '.js');
    if (context.length > 0) {
      entry = filesInModulesDir[index].replace(context, '').replace('.js', '');
    }
    manyEntries[entry] = path.resolve(filesInModulesDir[index]);
  }
  return manyEntries;
};

const minimizers = [];
if (mode === 'production') {
  minimizers.push(new TerserPlugin());
}

const plugins = [];
plugins.push(new ESLintPlugin({
  fix: true,
}));
if (mode !== 'production') {
  plugins.push(new BundleAnalyzer.BundleAnalyzerPlugin({
    analyzerMode: 'static',
    logLevel: 'silent',
    openAnalyzer: false,
  }));
}

module.exports = {
  mode: mode,
  entry: allEntries(),
  optimization: {
    splitChunks: false,
    minimizer: minimizers
  },
  plugins: plugins,
  module: {
    rules: [
      {
        test: /\.m?js$/,
        exclude: /(node_modules)/,
        use: {
          loader: 'babel-loader'
        }
      }
    ]
  }
};
