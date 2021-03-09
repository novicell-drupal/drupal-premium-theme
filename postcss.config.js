const env = process.env.NODE_ENV || 'development';

module.exports = {
  env,
  map: env !== 'production' ? {
    inline: false,
  } : false,
  plugins: [
    require('stylelint')({
      ignoreFiles: [
        'node_modules/**/*.css',
      ],
    }),
    require('postcss-preset-env')({
      stage: 3,
      preserve: false,
      autoprefixer: {
        grid: true,
      },
      features: {
        'custom-media-queries': true,
      },
    }),
    require('postcss-nested'),
    require('cssnano')({
      autoprefixer: false,
      discardComments: {
        removeAll: true,
      },
      mergeLonghand: true,
      colormin: false,
      zindex: false,
      discardUnused: {
        fontFace: false,
      },
    }),
    require('postcss-reporter')({
      clearReportedMessages: true,
      throwError: false,
    }),
  ],
};
