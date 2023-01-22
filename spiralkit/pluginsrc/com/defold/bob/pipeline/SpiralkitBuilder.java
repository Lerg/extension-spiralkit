package com.dynamo.bob.pipeline;

import com.dynamo.bob.BuilderParams;
import com.dynamo.bob.CompileExceptionError;
import com.dynamo.bob.ProtoBuilder;
import com.dynamo.bob.ProtoParams;
import com.dynamo.bob.Task;
import com.dynamo.bob.fs.IResource;
import com.dynamo.bob.pipeline.BuilderUtil;
import com.dynamo.spiralkit.proto.Spiralkit.SpiralkitDesc;

@ProtoParams(srcClass = SpiralkitDesc.class, messageClass = SpiralkitDesc.class)
@BuilderParams(name="Spiralkit", inExts=".spiralkit", outExt=".spiralkitc")
public class SpiralkitBuilder extends ProtoBuilder<SpiralkitDesc.Builder> {
	@Override
	protected SpiralkitDesc.Builder transform(Task<Void> task, IResource resource, SpiralkitDesc.Builder builder) throws CompileExceptionError {
		// Add any transforms here
		return builder;
	}
}
