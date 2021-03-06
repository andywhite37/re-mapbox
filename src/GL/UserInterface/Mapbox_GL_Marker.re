module Anchor = {
  type t =
    | Center
    | Top
    | Bottom
    | Left
    | Right
    | TopLeft
    | TopRight
    | BottomLeft
    | BottomRight;

  type t_js = string;

  let tToJs = (data) => switch data {
  | Center => "center"
  | Top => "top"
  | Bottom => "bottom"
  | Left => "left"
  | Right => "right"
  | TopLeft => "top-left"
  | TopRight => "top-right"
  | BottomLeft => "bottom-left"
  | BottomRight => "bottom-right"
  };
};

module Config = {
  type rendering =
    | Custom(Dom.element)
    | Color(string);

  let getElement = (r) => switch r {
  | Custom(el) => Some(el)
  | Color(_) => None
  };

  let getColor = (r) => switch r {
  | Custom(_) => None
  | Color(color) => Some(color)
  };

  type t = {
    anchor: option(Anchor.t),
    rendering: option(rendering)
    /* TODO: offset: option(PointLike.t) */
  };

  type t_js = {.
    "element": Js.undefined(Dom.element),
    "anchor": Js.undefined(Anchor.t_js),
    "color": Js.undefined(string)
  };

  let make = (~anchor=?, ~rendering=?, ()) =>
    { anchor, rendering };

  let empty = make(());

  [@bs.obj] external makeConfig : (
    ~element: Dom.element=?,
    ~anchor: Anchor.t_js=?,
    ~color: string=?
  ) => _ = "";

  let tToJs = (data): t_js =>
    makeConfig(
      ~element= ?data.rendering |. Belt.Option.flatMap(getElement),
      ~anchor= ?data.anchor |. Belt.Option.map(Anchor.tToJs),
      ~color= ?data.rendering |. Belt.Option.flatMap(getColor)
    );
};

type t = {.
  [@bs.meth] "setLngLat": Mapbox_GL_LngLat.t => t,
  [@bs.meth] "getElement": unit => Dom.element,
  [@bs.meth] "addTo": Mapbox_GL_Map.t => t,
  [@bs.meth] "remove": unit => t
};

[@bs.new][@bs.module "mapbox-gl/dist/mapbox-gl.js"]
external create: Config.t_js => t = "Marker";
