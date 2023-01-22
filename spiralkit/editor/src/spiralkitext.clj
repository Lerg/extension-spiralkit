(ns editor.spiralkit
  (:require [dynamo.graph :as g]
            [editor.build-target :as bt]
            [editor.graph-util :as gu]
            [editor.protobuf :as protobuf]
            [editor.resource-node :as resource-node]
            [editor.types :as types]
            [editor.validation :as validation]
            [editor.workspace :as workspace]))

(set! *warn-on-reflection* true)

(def ^:private spiralkit-ext "spiralkit")
(def ^:private spiralkit-icon "/spiralkit/editor/resources/icons/icon-spiralkit.png")
(def ^:private spiralkit-template "/spiralkit/editor/resources/templates/template.spiralkit")

(def ^:private spiralkit-plugin-desc-cls
  (delay (workspace/load-class! "com.dynamo.spiralkit.proto.Spiralkit$SpiralkitDesc")))

;; /////////////////////////////////////////////////////////////////////////////////////////////

;; Helper function for property validation. Applies the supplied validate-fn to
;; the value and the property name, and returns an ErrorValue in case it returns
;; a non-nil string expressing a problem with the value.
(defn- validate-property [prop-kw validate-fn node-id value]
  (validation/prop-error :fatal node-id prop-kw validate-fn value (validation/keyword->name prop-kw)))

;; Property validate-fn for use with the validate-property function. Takes a
;; property value and the name of the property. Is expected to test the validity
;; of the property value, and return a string describing the problem in case the
;; value is not valid. For valid values, it should return nil.
(defn- prop-non-empty? [value prop-name]
  (when (empty? value)
    (format "'%s' must be specified" prop-name)))

;; These all validate a single property and produce a human-readable error
;; message if the value is invalid. They are used for validation in the
;; Property panel, and to produce build errors when building the project.
(defn- validate-name [node-id value]
  (validate-property :name prop-non-empty? node-id value))

(defn- validate-f32 [node-id value]
  (validate-property :f32 validation/prop-1-1? node-id value))

(defn- validate-u32 [node-id value]
  (validate-property :u32 validation/prop-negative? node-id value))

(defn- validate-u64 [node-id value]
  (validate-property :u64 validation/prop-negative? node-id value))

;; Build function embedded in the build targets for SimpleData. Once build
;; targets have been gathered, this function will be called with a BuildResource
;; (for output), and the user-data from a SimpleData build target produced by
;; the produce-build-targets defnk. It's expected to return a map containing the
;; BuildResource and the content that should be written to it as a byte array.
(defn- build-spiralkit [resource _dep-resources user-data]
  (let [spiralkit-pb (:spiralkit-pb user-data)
        content (protobuf/map->bytes @spiralkit-plugin-desc-cls spiralkit-pb)]
    {:resource resource
     :content content}))

;; Produce the build targets for a single SimpleData resource. Each SimpleData
;; resource results in one binary resource for the engine runtime. The contents
;; of the build target are hashed and used to determine if we need to re-run the
;; build-fn and write a new file. If there are build errors, return an
;; ErrorValue that will abort the build and report the errors to the user.
(g/defnk produce-build-targets [_node-id resource spiralkit-pb own-build-errors]
  (g/precluding-errors own-build-errors
    [(bt/with-content-hash
       {:node-id _node-id
        :resource (workspace/make-build-resource resource)
        :build-fn build-spiralkit
        :user-data {:spiralkit-pb spiralkit-pb}})]))

;; Callback invoked by the form view when a value is edited by the user. Is
;; expected to perform the relevant changes to the graph. In our case, we simply
;; set the value of the property on the edited SimpleDataNode.
(defn- set-form-op! [user-data property-path value]
  (assert (= 1 (count property-path)))
  (let [node-id (:node-id user-data)
        prop-kw (first property-path)]
    (g/set-property! node-id prop-kw value)))

;; Callback invoked by the form view when a value is cleared (or reset), by the
;; user. Is expected to perform the relevant changes to the graph. In our case,
;; we simply clear the value of the property on the edited SimpleDataNode.
(defn- clear-form-op! [user-data property-path]
  (assert (= 1 (count property-path)))
  (let [node-id (:node-id user-data)
        prop-kw (first property-path)]
    (g/clear-property! node-id prop-kw)))

;; Produce form-data for editing SimpleData using the form view. This can be
;; used to open standalone SimpleData resources in an editor tab. The form view
;; will render a simple user-interface based on the data we return here.
(g/defnk produce-form-data [_node-id name]
  {:navigation false
   :form-ops {:user-data {:node-id _node-id}
              :set set-form-op!
              :clear clear-form-op!}
   :sections [{:title "Spiralkit"
               :fields [{:path [:name]
                         :label "Name"
                         :type :string}]}]
   :values {[:name] name}})

;; Produce a Clojure map representation of the protobuf field values that can be
;; saved to disk in protobuf text format, or built into a binary protobuf
;; message for the engine runtime.
(g/defnk produce-spiralkit-pb [name]
  {:name name})

;; Produce an ErrorPackage of one or more ErrorValues that express problems with
;; our SimpleData. If there are no errors, produce nil. Any errors produced here
;; will be reported as clickable errors in the Build Errors view.
(g/defnk produce-own-build-errors [_node-id name]
  (g/package-errors
    _node-id
    (validate-name _node-id name)))

;; After a SimpleDataNode has been created for our SimpleData resource, this
;; function is called with our self node-id and a Clojure map representation of
;; the protobuf data read from our resource. We're expected to return a sequence
;; of transaction steps that populate our SimpleDataNode from the protobuf data.
;; In our case, that simply means setting the property values on our node to the
;; values from the protobuf data.
(defn- load-spiralkit [_project self _resource data]
  (g/set-property
    self
    :name (:name data)))

;; Defines a node type that will represent SimpleData resources in the graph.
;; Whenever we encounter a .simpledata file in the project, a SimpleDataNode is
;; created for it, and the load-fn we register for the resource type will be run
;; to populate the SimpleDataNode from the protobuf data. We implement a series
;; of named outputs to make it possible to edit the node using the form view,
;; save changes, build binaries for the engine runtime, and so on.
(g/defnode SpiralkitNode
  (inherits resource-node/ResourceNode)

  ;; Editable properties.
  (property name g/Str (dynamic error (g/fnk [_node-id name] (validate-name _node-id name))))

  ;; Outputs for internal use.
  (output spiralkit-pb g/Any produce-spiralkit-pb)

  ;; Outputs we're expected to implement.
  (output form-data g/Any :cached produce-form-data)
  (output save-value g/Any (gu/passthrough spiralkit-pb))
  (output own-build-errors g/Any produce-own-build-errors)
  (output build-targets g/Any :cached produce-build-targets))

;; /////////////////////////////////////////////////////////////////////////////////////////////

;; Register our .simpledata resource type with the workspace. Whenever we find a
;; .simpledata file in the project, a SimpleDataNode is created for it. Then,
;; the load-fn populates the SimpleDataNode from a Clojure map representation of
;; the protobuf data we load from the .simpledata resource. When we register our
;; resource type, we also tag ourselves as a component that can be used in game
;; objects, and declare which view types can present our resource for editing in
;; an editor tab. In our case, we will use the form view for editing. To work
;; with the form view, our node is expected to implement the form-data output
;; required by the form view.
(defn- register-resource-types [workspace]
  (resource-node/register-ddf-resource-type
    workspace
    :ext spiralkit-ext
    :label "Spiralkit"
    :node-type SpiralkitNode
    :ddf-type @spiralkit-plugin-desc-cls
    :load-fn load-spiralkit
    :icon spiralkit-icon
    :view-types [:cljfx-form-view :text]
    :view-opts {}
    :tags #{:component}
    :tag-opts {:component {:transform-properties #{}}}
    :template spiralkit-template))

;; The plugin
(defn- load-plugin-spiralkit [workspace]
  (g/transact (register-resource-types workspace)))

(defn- return-plugin []
  (fn [workspace]
    (load-plugin-spiralkit workspace)))

(return-plugin)
