Pooling
=============

MaxPooling1D
------------

.. doxygenfunction:: MaxPool1D

Example:

.. code-block:: c++
    :linenos:

    ...
    l = Reshape(l,{1,784}); //image as a 1D signal with depth=1
    l = Conv1D(l,16, {3},{1});
    l = ReLu(l);
    l = MaxPool1D(l,{4},{4});  //MaxPool 4 stride 4
    ...


MaxPooling
----------

.. doxygenfunction:: MaxPool


Example:

.. code-block:: c++
   :linenos:

    ...
    l = Reshape(l,{1,28,28});
    l = Conv(l,32, {3,3},{1,1});
    l = ReLu(l);
    l = MaxPool(l,{3,3}, {1,1}, "same");
    ...


GlobalMaxPooling
-----------------

.. doxygenfunction:: GlobalMaxPool


Example:

.. code-block:: c++
   :linenos:

    ...
    l = MaxPool(ReLu(Conv(l,32,{3,3},{1,1})),{2,2});
    l = MaxPool(ReLu(Conv(l,64,{3,3},{1,1})),{2,2});
    l = MaxPool(ReLu(Conv(l,128,{3,3},{1,1})),{2,2});
    l = GlobalMaxPool(l);
    ...


AveragePooling
--------------

.. doxygenfunction:: AveragePool



GlobalAveragePooling
--------------------

.. doxygenfunction:: GlobalAveragePool

.. code-block:: c++
    :linenos:

    ...
    l = ResBlock(l, 512,2,1);
    l = ResBlock(l, 512,2,0);

    l = GlobalAveragePool(l);
    ...


