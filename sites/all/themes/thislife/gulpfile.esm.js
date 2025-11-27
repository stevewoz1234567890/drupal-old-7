import gulp from "gulp";
import less from "gulp-less";
import minify from "gulp-minify";

const paths = {
  styles: {
    src: "./css/main.less",
    watch: [
      "./css/archive.less",
      "./css/main.less",
      "./css/nav.less",
      "./css/player.less",
      "./css/prefixer.less",
      "./css/variables.less",
    ],
    dest: "./css/",
  },
  scripts: {
    src: ["./js/main.js", "./js/plugins.js"],
    dest: "./js/min/",
  },
};

export function styles() {
  return gulp
    .src(paths.styles.src)
    .pipe(less())
    .pipe(gulp.dest(paths.styles.dest));
}

export function scripts() {
  return gulp
    .src(paths.scripts.src)
    .pipe(
      minify({
        mangle: false,
        ext: {
          min: ".min.js",
        },
        noSource: true,
      })
    )
    .pipe(gulp.dest(paths.scripts.dest));
}

export function watch() {
  gulp.watch(paths.styles.watch, styles);
  gulp.watch(paths.scripts.src, scripts);
}

export default watch;
