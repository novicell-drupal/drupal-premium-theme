import LazyLoad from 'novicell-lazyload';
import debounce from 'lodash/debounce';

const lazy = new LazyLoad({
  includeWebp: true,
});

document.addEventListener('lazybeforeunveil', (event) => {
  lazy.lazyLoad(event);
}, true);

window.addEventListener('resize', debounce(() => {
  lazy.checkImages();
}, 150), false);
